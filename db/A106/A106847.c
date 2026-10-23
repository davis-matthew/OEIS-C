/*
 * A106847  a(n) = Sum {k + j*m <= n} (k + j*m), with 0 < k,j,m <= n.
 * https://oeis.org/A106847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k + j*m <= n, 0<k,j,m<=n} (k + j*m)
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 1229, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: double‐loop over m and j, summing the contributions
   for each pair (m,j).  For each (m,j) let k_max = n - j*m,
   then contribution = sum_{k=1..k_max} (k + j*m) = k_max*(k_max+1)/2 + j*m*k_max. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t m = 1; m <= n; ++m) {
        /* j must satisfy j*m <= n-1 (so k_max >= 1) */
        idx_t maxj = (n == 0) ? 0 : (n - 1) / m;

        for (idx_t j = 1; j <= maxj; ++j) {
            idx_t kmax = n - j * m;            /* positive by construction   */
            seq_t kmax_s = (seq_t)kmax;

            seq_t part1 = kmax_s * (kmax_s + 1) / 2;    /* sum k          */
            seq_t part2 = (seq_t)j * (seq_t)m * kmax_s;/* j*m * kmax     */

            result += part1 + part2;
        }
    }
    return result;
}
