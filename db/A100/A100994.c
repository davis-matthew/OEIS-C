/*
 * A100994  If n is a prime power p^m, m >= 1, then n, otherwise 1.
 * https://oeis.org/A100994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is a prime power p^m (m ≥ 1), 1 otherwise
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 107 terms through n = 941 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: test whether n (n > 1) is a prime power.
   Returns true if n = p^k for some prime p and integer k ≥ 1. */
static bool is_prime_power_idx(idx_t n)
{
    if (n <= 1) return false;          /* 1 is not a prime power */
    idx_t p = 0;

    /* Find the smallest prime factor p of n */
    for (idx_t i = 2; i <= n / i; ++i) /* i*i <= n , avoid overflow */
    {
        if (n % i == 0) { p = i; break; }
    }

    if (p == 0) return true;           /* n is prime */

    /* Divide by p until it's no longer divisible */
    while (n % p == 0) n /= p;

    return n == 1;                     /* n was p^k */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;            /* a(1) = 1 */
    return (is_prime_power_idx(n)) ? (seq_t)n : (seq_t)1;
}
