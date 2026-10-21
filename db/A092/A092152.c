/*
 * A092152  Sign of A092673(n).
 * https://oeis.org/A092152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = mu(n) for odd n * a(n) = sign(mu(n) - mu(n/2)) for even n
 *            * where mu is the Möbius function
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the Möbius function mu(x).  Returns -1, 0, or 1. */
static seq_t mobius(idx_t x)
{
    if (x == 1) return 1;
    seq_t sign = 1;
    idx_t t = x;

    /* factor 2 */
    if ((t & 1) == 0) {
        int cnt = 0;
        while ((t & 1) == 0) {
            t >>= 1;
            ++cnt;
            if (cnt > 1) return 0;  /* squared prime factor */
        }
        sign = -sign;                 /* one factor of 2 */
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
                if (cnt > 1) return 0;   /* squared prime factor */
            }
            sign = -sign;                 /* one odd prime factor */
        }
    }

    if (t > 1)           /* remaining prime factor */
        sign = -sign;

    return sign;
}

/* Return the required term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of offset, value is irrelevant */

    if ((n & 1) != 0) {         /* odd index */
        return mobius(n);
    } else {                    /* even index: n = 2*m */
        seq_t mu_n     = mobius(n);          /* mu(2*m) */
        seq_t mu_half  = mobius(n >> 1);      /* mu(m) */
        seq_t diff = mu_n - mu_half;
        return (diff > 0 ? 1 : (diff < 0 ? -1 : 0));
    }
}
