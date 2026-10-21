/*
 * A066086  Greatest common divisor of product (p-1) and product (p+1), where
 *          p ranges over distinct prime divisors of n; a(n) = gcd(A048250(n),
 *          A173557(n)).
 * https://oeis.org/A066086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( ∏_{p|n; p prime} (p-1), ∏_{p|n; p prime} (p+1) )
 * Range:     u8 n <= 909, u16 n <= 9632, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple trial‑division factorisation of n
 * and an Euclidean gcd that works for any seq_t that supports
 * the ordinary arithmetic operators and the % operator. */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* n >= 1 always, but guard against the trivial case */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Work with 64‑bit integers for the factorisation. */
    uint64_t tmp = (uint64_t)n;
    seq_t prod_minus = (seq_t)1;
    seq_t prod_plus  = (seq_t)1;

    /* Trial division: find distinct prime factors of n. */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* p is a prime divisor.  Include it once. */
            prod_minus *= (seq_t)(p - 1);
            prod_plus  *= (seq_t)(p + 1);
            /* Remove all powers of p from tmp. */
            while (tmp % p == 0) {
                tmp /= p;
            }
        }
    }
    /* If a prime factor larger than sqrt(original n) remains. */
    if (tmp > 1) {
        uint64_t p = tmp;
        prod_minus *= (seq_t)(p - 1);
        prod_plus  *= (seq_t)(p + 1);
    }

    /* Euclidean algorithm for gcd. */
    seq_t a0 = prod_minus;
    seq_t b0 = prod_plus;
    while (b0 != (seq_t)0) {
        seq_t r = a0 % b0;
        a0 = b0;
        b0 = r;
    }
    return a0;
}
