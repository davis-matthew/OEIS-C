/*
 * A087711  a(n) = smallest number k such that both k-n and k+n are primes.
 * https://oeis.org/A087711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest integer k such that (k-n) and (k+n) are both
 *            prime
 * Range:     u8 n <= 244, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t and idx_t type, using only 64‑bit arithmetic for primality checks. */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic prime test for 64‑bit values. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Handle the offset case: a(0) = 2. */
    if (n == 0) return (seq_t)2;

    /* The smallest possible candidate k is n + 2, because k-n must be at least 2. */
    seq_t k = (seq_t)n + 2ULL;

    for (;; ++k) {
        uint64_t a = (uint64_t)(k - (seq_t)n); /* k - n, always positive for k >= n+2 */
        uint64_t b = (uint64_t)(k + (seq_t)n); /* k + n, always > k >= n+2 */
        if (is_prime_uint64(a) && is_prime_uint64(b))
            return k;
    }
}
