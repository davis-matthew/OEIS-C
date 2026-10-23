/*
 * A167318  Totally multiplicative sequence with a(p) = 8*(p-3) for prime p.
 * https://oeis.org/A167318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (8*(p-3))^e with a(1)=1, and a(n)=0 if 3
 *            divides n
 * Range:     i8 n <= 7, i16 n <= 63, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fast exit for multiples of 3 (factor 3 gives zero). */
    if (n % 3 == 0) return 0;

    seq_t result = 1;
    uint64_t t = (uint64_t)n;          /* work in uint64_t for the factorisation */

    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t exp = 0;
            while (t % p == 0) {
                t /= p;
                exp++;
            }
            seq_t base = 8 * ((seq_t)p - 3);
            seq_t pow  = 1;
            while (exp--) pow = pow * base;
            result = result * pow;
        }
    }

    /* If the remaining t is a prime > sqrt(original n). */
    if (t > 1) {
        seq_t base = 8 * ((seq_t)t - 3);
        result = result * base;   /* exponent is 1 */
    }

    return result;
}
