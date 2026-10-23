/*
 * A107473  Sum of numerator and denominator of Product_{p|n, p prime} (1 -
 *          1/p).
 * https://oeis.org/A107473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator + denominator of ∏_{p|n} (1-1/p) in lowest
 *            terms
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: greatest common divisor for arbitrary‐precision seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Special case n=1 gives product 1/1 */
    if (n == (idx_t)1) {
        return (seq_t)2;   /* 1 + 1 */
    }

    idx_t tmp = n;
    seq_t rad = (seq_t)1;      /* product of distinct primes */
    seq_t phi = (seq_t)1;      /* product of (p-1) for distinct primes */

    for (idx_t p = (idx_t)2; p <= tmp / p; ++p) {
        if (tmp % p == (idx_t)0) {
            /* distinct prime factor found */
            rad *= (seq_t)p;
            phi *= (seq_t)(p - (idx_t)1);
            while (tmp % p == (idx_t)0) {
                tmp /= p;
            }
        }
    }
    if (tmp > (idx_t)1) {
        /* remaining prime factor */
        rad *= (seq_t)tmp;
        phi *= (seq_t)(tmp - (idx_t)1);
    }

    seq_t g = gcd_seq(phi, rad);
    return (phi / g) + (rad / g);
}
