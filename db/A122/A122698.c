/*
 * A122698  a(1)=a(2)=1 then a(n) = Sum_{d|n, 1<d<n} a(d)*a(n/d).
 * https://oeis.org/A122698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122698
   Recurrence:  a(1)=a(2)=1;
   For n≥3, a(n)=Σ_{d|n,1<d<n} a(d)*a(n/d).
   It is known that a(n)=0 unless n is a power of two.
   For n = 2^(k+1) (k≥0) we have a(n)=Catalan(k). */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Handle the two initial values */
    if (n == 1 || n == 2)
        return (seq_t)1;

    /* If n is not a power of two, the term is zero */
    if ((n & (n - 1)) != 0)
        return (seq_t)0;

    /* Find the exponent m such that n = 2^m (m ≥ 1) */
    idx_t m = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        m++;
    }

    /* m = log2(n). For a(2^(k+1)), m = k+1  →  k = m-1 */
    idx_t k = m - 1;

    /* Return the k-th Catalan number */
    return catalan(k);
}
