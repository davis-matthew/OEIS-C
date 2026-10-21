/*
 * A099634  a(n) = gcd(P+p, P*p) where P is the largest and p the smallest
 *          prime factor of n.
 * https://oeis.org/A099634
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(P + p, P * p) where p is the smallest prime factor of
 *            n and P is the largest prime factor of n
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return (seq_t)a;
}

seq_t a(idx_t n)
{
    /*  Offset starts at 2 – undefined otherwise */
    if (n < 2)
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t p = 0;   /* smallest prime factor */
    uint64_t P = 0;   /* largest  prime factor */
    uint64_t temp = nn;

    /* Edge‑case: n == 1 would have no prime factors */
    if (nn == 1)
        return (seq_t)1;

    /* trial division up to sqrt(nn) */
    for (uint64_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            if (p == 0)
                p = i;          /* first divisor → smallest prime factor */
            P = i;              /* keep latest divisor → largest so far */
            while (temp % i == 0)
                temp /= i;
        }
    }

    /* whatever is left > 1 is also a prime factor */
    if (temp > 1) {
        if (p == 0)
            p = temp;          /* n itself is prime */
        P = temp;              /* largest prime factor */
    }

    /* GCD of (P+p) and (P*p) */
    uint64_t s = P + p;
    uint64_t prod = P * p;      /* fits in 64‑bit for the typical bounds */
    return gcd_uint64(s, prod);
}
