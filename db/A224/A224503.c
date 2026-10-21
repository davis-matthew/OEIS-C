/*
 * A224503  Smallest nontrivial prime power congruent to 1 mod n.
 * https://oeis.org/A224503
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1690, u32 n <= 7546, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest nontrivial prime power ≡ 1 (mod n) (by search). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Test whether x > 1 is a prime power: p^k for some prime p, k ≥ 1. */
static bool is_prime_power(seq_t x)
{
    if (x <= (seq_t)1) return false;
    if (is_prime_big(x)) return true;

    seq_t divisor = (seq_t)2;
    seq_t rem = x;

    while (divisor * divisor <= rem)
    {
        if (rem % divisor == (seq_t)0)
        {
            /* Strip all copies of this divisor. */
            while (rem % divisor == (seq_t)0)
                rem /= divisor;
            /* If nothing remains, x is a power of divisor. */
            return rem == (seq_t)1;
        }
        divisor += (divisor == (seq_t)2) ? (seq_t)1 : (seq_t)2; /* 2,3,5,7,... */
    }

    /* No divisor found → x is prime (handled) or 1. */
    return false;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)2) return (seq_t)0;

    for (idx_t t = (idx_t)1; ; ++t)
    {
        seq_t m = (seq_t)1 + ((seq_t)t * (seq_t)n);
        if (is_prime_power(m))
            return m;
    }
}
