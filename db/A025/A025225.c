/*
 * A025225  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 2.
 *          Also a(n) = (2^n)*C(n-1), where C = A000108 (Catalan numbers).
 * https://oeis.org/A025225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * Catalan(n-1) , n ≥ 1 With a(1) = 2 (since
 *            Catalan(0)=1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n≥1; for n=0 we return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^n and multiply by Catalan(n-1). */
    seq_t two_pow_n = pow2(n);
    seq_t cat       = catalan(n - 1);
    return two_pow_n * cat;
}
