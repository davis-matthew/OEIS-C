/*
 * A217523  Number of assembly trees for complete bipartite graph K_{n,n}.
 * https://oeis.org/A217523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 10. For n ≥ 3: a(n) = 2*(6*n^2 - 12*n
 *            + 5)*a(n-1) - (4*n^3 - 20*n^2 + 29*n - 10)*(n-1)*a(n-2) This
 *            recurrence follows from the OEIS definition a(n) = n!^2 * b(n)
 *            with the given rational recurrence for b(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;

    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)10;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t i_s = (seq_t)i;
        seq_t i_sq = i_s * i_s;
        seq_t i_cub = i_sq * i_s;

        /* c1 = 2*(6*i^2 - 12*i + 5) */
        seq_t c1 = (seq_t)2 * ((seq_t)6 * i_sq - (seq_t)12 * i_s + 5);

        /* c2_part = 4*i^3 - 20*i^2 + 29*i - 10 */
        seq_t c2_part = (seq_t)4 * i_cub - (seq_t)20 * i_sq +
                        (seq_t)29 * i_s - 10;

        /* c2 = c2_part * (i-1) */
        seq_t c2 = c2_part * (i_s - 1);

        /* a(i) = c1 * a(i-1) - c2 * a(i-2) */
        seq_t term1 = c1 * a_nm1;
        seq_t term2 = c2 * a_nm2;
        seq_t ai = term1 - term2;

        a_nm2 = a_nm1;
        a_nm1 = ai;
    }
    return a_nm1;
}
