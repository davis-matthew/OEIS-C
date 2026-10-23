/*
 * A173680  Number of ways of writing n as a sum of 6 nonnegative cubes.
 * https://oeis.org/A173680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of a(n) = coefficient of q^n in (∑_{m≥0} q^{m³})⁶,
   i.e. the number of ordered 6‑tuples of non‑negative cubes summing to n. */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Allocate array for dynamic programming, zero‑initialized. */
    seq_t *dp_prev = NULL;
#ifdef __cplusplus
    dp_prev = new seq_t[(size_t)n + 1];
    for (size_t i = 1; i <= (size_t)n; ++i) dp_prev[i] = (seq_t)0;
#else
    dp_prev = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
#endif
    if (!dp_prev) return 0;          /* unlikely */
    dp_prev[0] = (seq_t)1;

    /* Pre‑compute all cubes ≤ n.  The maximum cube needed for n≈10 000 is 21³=9261. */
    idx_t cubes[64];
    idx_t cubes_cnt = 0;
    for (idx_t m = 0;; ++m) {
        idx_t c = m * m * m;
        if (c > n) break;
        cubes[cubes_cnt++] = c;
    }

    /* Perform six successive convolutions. */
    for (idx_t step = 0; step < 6; ++step) {
        seq_t *dp_cur = NULL;
#ifdef __cplusplus
        dp_cur = new seq_t[(size_t)n + 1];
        for (size_t i = 0; i <= (size_t)n; ++i) dp_cur[i] = (seq_t)0;
#else
        dp_cur = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
#endif
        if (!dp_cur) {
            /* clean up and abort on allocation failure. */
#ifdef __cplusplus
            delete[] dp_prev;
#else
            free(dp_prev);
#endif
            return 0;
        }

        for (idx_t ci = 0; ci < cubes_cnt; ++ci) {
            idx_t c = cubes[ci];
            for (idx_t j = c; j <= n; ++j) {
                dp_cur[j] += dp_prev[j - c];
            }
        }

#ifdef __cplusplus
        delete[] dp_prev;
#else
        free(dp_prev);
#endif
        dp_prev = dp_cur;
    }

    seq_t result = dp_prev[n];
#ifdef __cplusplus
    delete[] dp_prev;
#else
    free(dp_prev);
#endif
    return result;
}
