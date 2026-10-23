/*
 * A014673  Smallest prime factor of greatest proper divisor of n.
 * https://oeis.org/A014673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime factor of the greatest proper divisor of n
 *            (i.e., a(n)=1 if n is prime, otherwise a(n)=p where n=p·q and p
 *            is the smallest prime factor of n)
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: return the smallest prime factor of a 64‑bit integer.
   Uses a simple trial division, which is fast enough for the values
   (indices) used in the tests. */
static uint64_t smallest_prime_factor_u64(uint64_t n)
{
    if (n <= 1) return n;          /* 0 or 1 maps to itself for safety */
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) return d;
    }
    return n;                      /* n is prime */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1U) return (seq_t)1;

    /* Compute the smallest prime factor p of n */
    uint64_t p = smallest_prime_factor_u64((uint64_t)n);

    /* If n itself is prime, a(n)=1 */
    if (p == (uint64_t)n) return (seq_t)1;

    /* Greatest proper divisor d = n / p */
    uint64_t d = (uint64_t)n / p;

    /* Smallest prime factor of d gives the desired value */
    uint64_t q = smallest_prime_factor_u64(d);

    return (seq_t)q;
}
