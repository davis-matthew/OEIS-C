/*
 * A211294  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 1,
 *          2] as of [2, 2, 1].
 * https://oeis.org/A211294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A211294: a(n) = number of n‑letter words over {1,2,3}
    containing equal numbers of the substrings 112 and 221.
    Implementation uses dynamic programming on the
    last two letters and the current difference
    (#112 – #221).  All operations are exact and
    work for any integer size of seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial cases – skip the DP */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 9;

    /* maximum possible absolute difference is floor(n/3) */
    size_t max_diff = n / 3;
    size_t sz = 2 * max_diff + 1;          /* array size for each state */
    size_t off = max_diff;                 /* index of difference 0 */

    /* DP arrays for 9 states (pairs of last two letters) */
    seq_t *curr[9], *next[9];
    for (int s = 0; s < 9; ++s) {
        curr[s] = (seq_t*)calloc(sz, sizeof(seq_t));
        next[s] = (seq_t*)calloc(sz, sizeof(seq_t));
    }

    /* initialise for length 2 – all pairs of letters are possible once */
    for (int s = 0; s < 9; ++s)
        curr[s][off] = 1;

    /* pre‑compute transitions and delta for each state and new letter */
    int new_state[9][4];
    int delta[9][4];
    for (int s = 0; s < 9; ++s) {
        int a = s / 3 + 1;          /* first letter of the pair (1‑3) */
        int b = s % 3 + 1;          /* second letter                 */
        for (int c = 1; c <= 3; ++c) {
            new_state[s][c] = (b - 1) * 3 + (c - 1);
            delta[s][c] = (a == 1 && b == 1 && c == 2) ?  1
                           : (a == 2 && b == 2 && c == 1) ? -1
                           : 0;
        }
    }

    /* dynamic programming for lengths 3 … n */
    for (idx_t len = 2; len < n; ++len) {          /* current length = len */
        /* zero next arrays */
        for (int s = 0; s < 9; ++s)
            for (size_t i = 0; i < sz; ++i)
                next[s][i] = 0;

        size_t curr_max = len / 3;                 /* max |diff| for this length */

        /* iterate over all states and possible next letters */
        for (int s = 0; s < 9; ++s) {
            seq_t *cs = curr[s];
            for (int c = 1; c <= 3; ++c) {
                int ns = new_state[s][c];
                int dlt = delta[s][c];
                seq_t *ns_arr = next[ns];
                for (int diff = -(int)curr_max; diff <= (int)curr_max; ++diff) {
                    seq_t val = cs[off + diff];
                    if (!val) continue;
                    size_t idx = off + diff + dlt;
                    ns_arr[idx] += val;
                }
            }
        }
        /* swap curr and next */
        for (int s = 0; s < 9; ++s) {
            seq_t *tmp = curr[s];
            curr[s] = next[s];
            next[s] = tmp;
        }
    }

    /* sum over all states for difference 0 */
    seq_t res = 0;
    for (int s = 0; s < 9; ++s)
        res += curr[s][off];

    /* free memory */
    for (int s = 0; s < 9; ++s) {
        free(curr[s]);
        free(next[s]);
    }

    return res;
}
