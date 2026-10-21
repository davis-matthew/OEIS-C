/*
 * A081282  Generalized centered polygonal numbers.
 * https://oeis.org/A081282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*C(n+3,5) + C(n+2,3)
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 190, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = n*(n+1)*(n+2)*(n*n+2*n+7)/60 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute factors */
    seq_t a_fac = (seq_t)n;
    seq_t b_fac = (seq_t)n + 1;
    seq_t c_fac = (seq_t)n + 2;
    seq_t d_fac = (seq_t)n * (seq_t)n + (seq_t)2 * (seq_t)n + (seq_t)7;

    /* divide by 2^2·3·5 (i.e., 60).  The product of the four
       factors is always divisible by 60, so one of the
       factors will be divisible by each prime factor. */
    const unsigned primes[4] = {2, 2, 3, 5};
    for (int i = 0; i < 4; ++i) {
        unsigned p = primes[i];
        if (a_fac % p == 0) {
            a_fac /= p;
        } else if (b_fac % p == 0) {
            b_fac /= p;
        } else if (c_fac % p == 0) {
            c_fac /= p;
        } else {
            d_fac /= p;  /* guaranteed to be divisible */
        }
    }

    /* final product */
    return a_fac * b_fac * c_fac * d_fac;
}
