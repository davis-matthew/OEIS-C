/*
 * A211277  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 1]
 *          as of [2, 2].
 * https://oeis.org/A211277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n‑letter words over {1,2,3} with equal * count
 *            of the substrings 11 and 22
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Range of possible difference values [-(n-1), +(n-1)] */
    size_t diff_range = 2 * (size_t)n + 1;  /* indices 0 … 2*n   */
    size_t offset     = (size_t)n;          /* index of difference 0 */

    /* dp_cur[letter][diff] and dp_next[letter][diff] */
    seq_t *dp_cur[3];
    seq_t *dp_next[3];

    /* Initial allocation of current DP table (all zero) */
    for (int i = 0; i < 3; ++i) {
#ifdef __cplusplus
        dp_cur[i] = new seq_t[diff_range]();   /* zero‑initialized */
#else
        dp_cur[i] = (seq_t *)calloc(diff_range, sizeof(seq_t));
#endif
        if (!dp_cur[i]) { /* safety: out of memory */
            for (int j = 0; j < i; ++j) {
#ifdef __cplusplus
                delete[] dp_cur[j];
#else
                free(dp_cur[j]);
#endif
            }
            return 0; /* fallback */
        }
    }

    /* Words of length 1: only one word ending in each letter, diff = 0 */
    dp_cur[0][offset] = 1; /* ends with 1 */
    dp_cur[1][offset] = 1; /* ends with 2 */
    dp_cur[2][offset] = 1; /* ends with 3 */

    /* Extend the word one character at a time */
    for (idx_t len = 1; len < n; ++len) {
        /* Allocate a new DP table for the next length */
        for (int i = 0; i < 3; ++i) {
#ifdef __cplusplus
            dp_next[i] = new seq_t[diff_range]();   /* zero‑initialized */
#else
            dp_next[i] = (seq_t *)calloc(diff_range, sizeof(seq_t));
#endif
            if (!dp_next[i]) {
                /* free what we have built so far */
                for (int k = 0; k < i; ++k) {
#ifdef __cplusplus
                    delete[] dp_cur[k];
#else
                    free(dp_cur[k]);
#endif
                }
                for (int k = 0; k < 3; ++k) {
#ifdef __cplusplus
                    delete[] dp_next[k];
#else
                    free(dp_next[k]);
#endif
                }
                return 0;
            }
        }

        /* Transition from all states of the current length to next length */
        for (size_t idx = 0; idx < diff_range; ++idx) {
            /* States ending with 1 */
            seq_t v = dp_cur[0][idx];
            if (v) {
                /* Append 1 -> diff +1 */
                if (idx + 1 < diff_range)
                    dp_next[0][idx + 1] += v;
                /* Append 2 or 3 -> diff unchanged */
                dp_next[1][idx] += v;
                dp_next[2][idx] += v;
            }

            /* States ending with 2 */
            v = dp_cur[1][idx];
            if (v) {
                /* Append 1 or 3 -> diff unchanged */
                dp_next[0][idx] += v;
                dp_next[2][idx] += v;
                /* Append 2 -> diff -1 */
                if (idx > 0)
                    dp_next[1][idx - 1] += v;
            }

            /* States ending with 3 */
            v = dp_cur[2][idx];
            if (v) {
                /* Append 1, 2, or 3 -> diff unchanged */
                dp_next[0][idx] += v;
                dp_next[1][idx] += v;
                dp_next[2][idx] += v;
            }
        }

        /* Free the old current table */
        for (int i = 0; i < 3; ++i) {
#ifdef __cplusplus
            delete[] dp_cur[i];
#else
            free(dp_cur[i]);
#endif
        }

        /* Swap pointers: new table becomes current */
        for (int i = 0; i < 3; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Sum all words of length n with difference 0 */
    seq_t result = dp_cur[0][offset] + dp_cur[1][offset] + dp_cur[2][offset];

    /* Free the final DP table */
    for (int i = 0; i < 3; ++i) {
#ifdef __cplusplus
        delete[] dp_cur[i];
#else
        free(dp_cur[i]);
#endif
    }

    return result;
}
