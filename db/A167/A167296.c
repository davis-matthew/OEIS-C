/*
 * A167296  Totally multiplicative sequence with a(p) = 4*(p-2) for prime p.
 * https://oeis.org/A167296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over odd prime powers p^e | n of (4*(p-2))^e.
 *            a(n) = 0 if n is even (since 2 is a factor). a(1) = 1
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper to compute b^e (with e small). */
static seq_t power_seq_t(seq_t b, unsigned e)
{
    seq_t r = 1;
    while (e--) r *= b;
    return r;
}

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 1ULL) return 1;              /* a(1) = 1 */

    /* Multiplication by 2 gives a zero factor. */
    if ((n & 1ULL) == 0ULL) return 0;     /* a(n) = 0 for even n */

    seq_t prod = 1;
    idx_t m = n;                          /* copy of n for factoring */

    /* Factor out the prime 3,5,7,... */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p != 0ULL) continue;

        unsigned e = 0;
        while (m % p == 0ULL) {
            m /= p;
            ++e;
        }

        seq_t base = (seq_t)4 * (seq_t)(p - 2ULL);
        prod *= power_seq_t(base, e);
    }

    /* If n had a remaining prime factor > sqrt(original n). */
    if (m > 1ULL) {
        seq_t base = (seq_t)4 * (seq_t)(m - 2ULL);
        prod *= base;                    /* exponent e=1 */
    }

    return prod;
}
