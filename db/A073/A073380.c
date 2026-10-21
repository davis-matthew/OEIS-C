/*
 * A073380  Third convolution of A000129(n+1) (generalized (2,1)-Fibonacci,
 *          called Pell numbers), n>=0, with itself.
 * https://oeis.org/A073380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 2^(n-2k) * binomial(n-k+3,3) *
 *            binomial(n-k,k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 40, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute 2^n.  This uses binary exponentiation to keep the number of
       multiplication operations small. */
    seq_t power = 1;
    seq_t two = 2;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            power *= two;
        two *= two;
        exp >>= 1;
    }

    seq_t result = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - k;                     /* m = n-k */

        /* C(m+3,3) = (m+3)*(m+2)*(m+1)/6  */
        seq_t c1 = (seq_t)(m + 3) * (seq_t)(m + 2) * (seq_t)(m + 1);
        c1 /= 6;

        /* C(m,k).  Use the multiplicative formula reducing the number of
           iterations by symmetry. */
        seq_t c2 = 1;
        idx_t kk = k;
        if (kk > m - kk)
            kk = m - kk;
        for (idx_t i = 1; i <= kk; ++i) {
            seq_t mult = (seq_t)(m - kk + i);
            c2 *= mult;
            c2 /= (seq_t)i;
        }

        seq_t term = power;
        term *= c1;
        term *= c2;
        result += term;

        /* Prepare power for the next k: 2^(n-2(k+1)) = 2^(n-2k-2) = power/4  */
        power /= 4;
    }

    return result;
}
