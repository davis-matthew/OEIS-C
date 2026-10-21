/*
 * A086668  Number of divisors d of n such that 2d+1 is a prime.
 * https://oeis.org/A086668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | n : 2*d + 1 is prime }
 * Range:     u8 n <= 9961, u16 n <= 9961, u32 n <= 9961, u64 n <= 9961,
 *            u128 n <= 9961, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Sequence: A086668

#include <stdbool.h>

static bool is_small_prime(uint64_t x)
{
    if (x < 2U) return false;
    if (x == 2U) return true;
    if ((x & 1U) == 0U) return false;
    for (uint64_t i = 3U; i * i <= x; i += 2U) {
        if (x % i == 0U) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence is defined for n ≥ 1. */
    if (n == 0U) return (seq_t)0U;  /* undefined, return 0 */

    uint64_t nn = (uint64_t)n;      /* index fits in 64‑bit unsigned */
    uint64_t half = nn >> 1;        /* not used, just a placeholder */

    seq_t cnt = (seq_t)0U;          /* result counter */

    for (uint64_t d = 1U; d * d <= nn; ++d) {
        if (nn % d == 0U) {
            /* divisor d */
            if (is_small_prime(2U * d + 1U)) ++cnt;

            uint64_t d2 = nn / d;
            if (d2 != d) {
                if (is_small_prime(2U * d2 + 1U)) ++cnt;
            }
        }
    }

    return cnt;
}
