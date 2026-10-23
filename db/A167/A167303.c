/*
 * A167303  Totally multiplicative sequence with a(p) = 2*(p+2) for prime p.
 * https://oeis.org/A167303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e||n of (2*(p+2))^e, * i.e.
 *            a(1)=1, a(p)=2*(p+2), a(n)=∏(2*(p+2))^e where n=∏p^e
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation uses trial division; fast enough for n up to ~10⁴
 * and for larger n a simple prime sieve could replace the loop without changing
 * the algorithmic idea. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is the first term of the sequence */
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;          /* running product */
    uint64_t remaining = (uint64_t)n; /* working copy of n */

    /* factor out small primes by trial division */
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            int exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            /* compute (2*(p+2))^exp */
            seq_t base = (seq_t)(2 * (p + 2));
            seq_t term = (seq_t)1;
            for (int i = 0; i < exp; ++i) term *= base;
            result *= term;
        }
    }

    /* if anything left > 1, it is a prime factor > sqrt(original n) */
    if (remaining > 1) {
        seq_t base = (seq_t)(2 * (remaining + 2));
        result *= base;
    }

    return result;
}
