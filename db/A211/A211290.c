/*
 * A211290  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 1,
 *          2] as of [1, 2, 2].
 * https://oeis.org/A211290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n‑letter words over {1,2,3} with * #( [1,1,2]
 *            substrings ) = #( [1,2,2] substrings ). * This implementation
 *            uses a dynamic programming state * (current length, last two
 *            letters, difference of counts). * Complexity: O(n) time and O(n)
 *            space
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The build system supplies the following types. */
extern seq_t a(idx_t n);                /* declaration kept for clarity */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* maximum possible difference is (n-2) for each side; the array
     * accommodates all diffs in the range [-(n-2), n-2]. */
    const int64_t off = (int64_t)n - 2;                 /* offset for array index */
    const size_t diff_range = (size_t)(2 * ((int64_t)n - 2) + 1);
    const size_t states = 9;                          /* 3 last letters × 3 current letters */

    const size_t total = diff_range * states;          /* total cells in each DP array */

    /* allocate DP arrays */
    seq_t *cur = (seq_t *)calloc(total, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(total, sizeof(seq_t));
    if (!cur || !next) {
        free(cur);
        free(next);
        return (seq_t)0;
    }

    /* initialise for length 2: every pair of letters occurs once,
     * with difference 0. */
    for (uint64_t a1 = 1; a1 <= 3; ++a1) {
        for (uint64_t a2 = 1; a2 <= 3; ++a2) {
            const size_t idx = ((size_t)(off) * states) + (size_t)((a1 - 1) * 3 + (a2 - 1));
            cur[idx] = (seq_t)1;
        }
    }

    /* extend the word one letter at a time */
    for (uint64_t len = 2; len < n; ++len) {
        /* reset next array */
        for (size_t i = 0; i < total; ++i) next[i] = 0;

        const int64_t diff_min = -( (int64_t)len - 2 );
        const int64_t diff_max =   ( (int64_t)len - 2 );

        for (int64_t diff = diff_min; diff <= diff_max; ++diff) {
            const size_t base_cur = (size_t)(diff + off) * states;
            for (uint64_t state = 0; state < states; ++state) {
                const seq_t val = cur[base_cur + state];
                if (val == 0) continue;

                /* recover last two letters from state index */
                const uint64_t first  = state / 3 + 1;   /* a */
                const uint64_t second = state % 3 + 1;   /* b */

                /* try all possible next letters c = 1,2,3 */
                for (uint64_t c = 1; c <= 3; ++c) {
                    int inc = 0;
                    if (first == 1 && second == 1 && c == 2)       inc =  1;
                    else if (first == 1 && second == 2 && c == 2) inc = -1;

                    const int64_t new_diff = diff + inc;
                    const uint64_t dest_state = (second - 1) * 3 + (c - 1);
                    next[(size_t)(new_diff + off) * states + dest_state] += val;
                }
            }
        }
        /* swap cur and next for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum counts where diff == 0 */
    seq_t result = 0;
    const size_t base = (size_t)(off) * states;
    for (uint64_t state = 0; state < states; ++state) {
        result += cur[base + state];
    }

    free(cur);
    free(next);
    return result;
}
