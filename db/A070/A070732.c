/*
 * A070732  Size of largest conjugacy class in the group GL(2,Z_n).
 * https://oeis.org/A070732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^e|| * ((p+1) * p^(2e-k)),
 *            where k = 1 if p odd, * k = 2 if p = 2 (this is the size of the
 *            largest * conjugacy class in GL(2, Z_n)
 * Range:     u8 n <= 14, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) is undefined, but return 0 safely */
    seq_t result = 1;

    idx_t x = n;
    /* handle factor 2 separately because of special k=2 */
    idx_t e = 0;
    while ((x & 1ULL) == 0) {       /* x % 2 == 0 */
        x >>= 1;                    /* x /= 2 */
        ++e;
    }
    if (e) {
        idx_t exp = 2 * e - 2;      /* k = 2 for p = 2 */
        seq_t pow2 = 1;
        for (idx_t i = 0; i < exp; ++i)
            pow2 *= (seq_t)2;
        seq_t term = ((seq_t)3) * pow2;  /* (2+1)*2^(exp) */
        result *= term;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            idx_t exp = 2 * e - 1;          /* k = 1 for odd p */
            seq_t ppow = 1;
            for (idx_t i = 0; i < exp; ++i)
                ppow *= (seq_t)p;
            seq_t term = ((seq_t)(p + 1)) * ppow;
            result *= term;
        }
    }

    /* remaining prime factor > sqrt */
    if (x > 1) {
        /* x is prime, exponent e = 1 */
        seq_t term = ((seq_t)(x + 1)) * (seq_t)x;   /* (p+1)*p^(1) */
        result *= term;
    }

    return result;
}
