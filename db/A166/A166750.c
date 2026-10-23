/*
 * A166750  a(n) = (A001147(n))^3 = 2^(3*n)*Gamma(n+1/2)^3/Pi^(3/2).
 * https://oeis.org/A166750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n-1)!!)^3 (product_{k=1..n} (2k-1)^3, a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;                       /* a(0) = 1 */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t odd = ((seq_t)2 * k) - 1;  /* current odd number */
        res = res * odd * odd * odd;    /* multiply by odd^3 */
    }
    return res;
}
