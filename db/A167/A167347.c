/*
 * A167347  Totally multiplicative sequence with a(p) = (p-1)*(p-3) = p^2-4p+3
 *          for prime p.
 * https://oeis.org/A167347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product_{p^e||n} ((p-1)*(p-3))^e (with a(3)=0, so any
 *            factor 3 yields a(n)=0)
 * Range:     i8 n <= 16, i16 n <= 190, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety, offset starts at 1 */
    uint64_t tmp = (uint64_t)n;         /* local copy for factorisation */
    seq_t result = 1;

    while (tmp > 1) {
        uint64_t p;                     /* smallest prime factor */

        if ((tmp & 1ULL) == 0) {        /* handle factor 2 */
            p = 2;
        } else {                       /* odd factors */
            p = tmp;
            for (uint64_t d = 3; d * d <= tmp; d += 2) {
                if (tmp % d == 0) {    /* found smallest odd factor */
                    p = d;
                    break;
                }
            }
        }

        /* count the exponent of p */
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }

        /* compute a(p) = p^2 - 4p + 3 = (p-1)*(p-3) */
        seq_t fp = (seq_t)p * (seq_t)p - ((seq_t)4 * (seq_t)p) + 3;

        /* multiply result by fp^exp */
        for (uint64_t i = 0; i < exp; ++i) {
            result *= fp;
            if (fp == 0) return 0;      /* any factor 3 gives zero */
        }
    }

    return result;
}
