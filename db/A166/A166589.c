/*
 * A166589  Totally multiplicative sequence with a(p) = p-3 for prime p.
 * https://oeis.org/A166589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Π (p-3)^{e} over all prime powers p^{e} || n
 *            (multiplicative; a(3k)=0, a(2^k)=(-1)^k, etc.)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* zero is undefined for this sequence (offset=1) */
    if (n < 1) return 0;

    /* a(1)=1 (empty product) */
    if (n == 1) return 1;

    /* any factor 3 gives zero */
    if (n % 3 == 0) return 0;

    uint64_t m = (uint64_t)n;          /* m is the part still to factor */
    seq_t result = 1;

    /* factor out 2 first */
    if ((m & 1) == 0) {
        unsigned exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            exp++;
        }
        seq_t base = (seq_t)2 - 3;    /* -1 */
        /* base is negative, but power is positive integer */
        seq_t power = 1;
        for (unsigned i = 0; i < exp; ++i)
            power *= base;           /* power = (-1)^exp */
        result *= power;
    }

    /* factor odd primes */
    uint64_t p = 3;
    while ((p * p) <= m) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            seq_t base = (seq_t)p - 3;        /* signed */
            if (base == 0) return 0;          /* should not happen (p!=3) */
            seq_t power = 1;
            for (unsigned i = 0; i < exp; ++i)
                power *= base;
            result *= power;
        }
        p += 2;   /* skip even numbers */
    }

    /* if remainder m>1, it is a prime > sqrt(original n) */
    if (m > 1) {
        seq_t base = (seq_t)m - 3;
        if (base == 0) return 0;           /* m==3 would have been caught earlier */
        result *= base;                    /* exponent is 1 */
    }

    return result;
}
