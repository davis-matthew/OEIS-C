/*
 * A051677  Tetrahedron-tree numbers: a(n) = Sum_{m=1..n} b(m), b(m)=1, 1,3,
 *          1,3,6, 1,3,6,10, ..., 1,3,...,i*(i+1)/2.
 * https://oeis.org/A051677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, changed
 * Formula:   a(n) = Sum_{m=1..n} b(m), where b(k*(k+1)/2 + m) = m*(m+1)/2
 *            (m=1..k+1), k≥0 which simplifies to a(k*(k+1)/2 + m) =
 *            k*(k+1)*(k+2)*(k+3)/24 + m*(m+1)*(m+2)/6 (m = n - k*(k+1)/2, 0 ≤
 *            k such that k*(k+1)/2 ≤ n < (k+1)*(k+2)/2)
 * Range:     u8 n <= 33, u16 n <= 594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 1, return 0 for completeness */
        return 0;

    /* Find block index k such that T_k <= n < T_{k+1} */
    idx_t k = 0;
    while ((k + 1) * (k + 2) / 2 <= n)
        ++k;

    idx_t t_k = k * (k + 1) / 2;      /* triangular number T_k          */
    idx_t m   = n - t_k;              /* position inside the block      */

    /* Compute the two parts of the formula using seq_t arithmetic */
    seq_t part1 = ((seq_t)k * (k + 1) * (k + 2) * (k + 3)) / 24;
    seq_t part2 = ((seq_t)m * (m + 1) * (m + 2)) / 6;

    return part1 + part2;
}
