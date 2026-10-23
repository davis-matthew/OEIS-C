/*
 * A197433  Sum of distinct Catalan numbers: a(n) = Sum_{k>=0}
 *          A030308(n,k)*C(k+1) where C(n) is the n-th Catalan number
 *          (A000108). (C(0) and C(1) not treated as distinct.)
 * https://oeis.org/A197433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k≥0} bit_k(n) * Catalan(k+1) (bit_k(n) = 1 iff the
 *            k‑th binary digit of n is 1. )
 * Range:     u8 n <= 31, u16 n <= 1090, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t k = 0;
    idx_t m = n;
    while (m)
    {
        if (m & 1ULL)          /* set bit at position k */
            result += catalan(k + 1);
        m >>= 1;
        ++k;
    }
    return result;
}
