/*
 * A075906  Seventh column of triangle A075498.
 * https://oeis.org/A075906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075906: a(n) = 3^n * S2(n+7, 7), where S2 is the Stirling number of the second kind */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Compute Stirling number S2(n+7, 7) using a DP that keeps only the current row. */
    seq_t S[8] = {0};        /* S[k] holds S2(current_i, k) */
    S[0] = 1;                /* S2(0,0) = 1 */

    idx_t m = n + 7;         /* we need to go up to n+7 */
    for (idx_t i = 1; i <= m; ++i) {
        /* Update from high k to low to avoid using temporary array. */
        for (int k = 7; k >= 1; --k) {
            /* S[k] is S2(i-1, k) before the assignment. */
            S[k] = (seq_t)k * S[k] + S[k-1];
        }
        S[0] = 0;            /* S2(i,0) = 0 for i > 0 */
    }

    /* Compute 3^n */
    seq_t pow3 = 1;
    for (idx_t k = 0; k < n; ++k) {
        pow3 *= (seq_t)3;
    }

    /* Result */
    return pow3 * S[7];
}
