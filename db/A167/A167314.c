/*
 * A167314  Totally multiplicative sequence with a(p) = 4*(p-3) for prime p.
 * https://oeis.org/A167314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Prod_{p^e || n} (4*(p-3))^e (Totally multiplicative)
 * Range:     i8 n <= 15, i16 n <= 255, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(3k) = 0 for k >= 1                         */
/*          a(1) = 1                                      */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 1) return 1;
    if (n % 3 == 0) return 0;          /* base at p = 3 is 0 */

    seq_t result = 1;
    uint64_t nn  = (uint64_t)n;

    /* factor 2 separately */
    while ((nn & 1ULL) == 0) {         /* while nn % 2 == 0 */
        result *= (seq_t)4 * ((seq_t)2 - 3);   /* (4*(2-3)) = -4 */
        nn >>= 1;                       /* nn /= 2 */
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        while (nn % p == 0) {
            result *= (seq_t)4 * ((seq_t)p - 3);
            nn /= p;
        }
    }

    /* if a prime factor remains */
    if (nn > 1) {
        result *= (seq_t)4 * ((seq_t)nn - 3);
    }

    return result;
}
