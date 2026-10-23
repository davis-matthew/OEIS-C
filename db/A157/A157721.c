/*
 * A157721  a(n) = 0 if n is 1 or a prime, otherwise a(n) = product of
 *          composite (nonprime) divisors of n.
 * https://oeis.org/A157721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = product of composite (nonprime) divisors of n, * zero if
 *            n == 1 or n is prime. * Implementation uses trial division for
 *            primality and * a binary‑search integer square root
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 71, u64 n <= 239,
 *            u128 n <= 719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* integer square root (floor) for unsigned 64‑bit integers
 *  - no floating point or <math.h> used
 *  - works for all n < 2^64
 */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1ULL << 32;  /* 2^32 > sqrt(2^64-1) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= n / mid)   /* mid*mid <= n without overflow */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(n)) */
}

/* simple primality test by trial division up to sqrt(n)
 *  - valid for n up to <2^64   */
static inline bool is_prime_fast(idx_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)       return false;
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)                    return (seq_t)0;
    if (is_prime_fast(n))          return (seq_t)0;

    seq_t prod = 1;
    idx_t limit = floor_sqrt_u64(n);

    for (idx_t i = 2; i <= limit; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (!is_prime_fast(d1))
                prod = prod * (seq_t)d1;
            if (d2 != d1 && !is_prime_fast(d2))
                prod = prod * (seq_t)d2;
        }
    }
    /* include n itself (composite) */
    prod = prod * (seq_t)n;

    return prod;
}
