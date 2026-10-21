/*
 * A071187  Smallest prime factor of number of divisors of n; a(1) = 1.
 * https://oeis.org/A071187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime factor of τ(n) with a(1)=1 where τ(n) is
 *            the number of divisors of n
 * Range:     u8 n <= 9618, u16 n <= 9618, u32 n <= 9618, u64 n <= 9618,
 *            u128 n <= 9618, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Simple trial‑division divisor count for uint64_t */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t tau = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
            }
            tau *= (uint64_t)(cnt + 1);
        }
    }
    if (n > 1) tau *= 2;   /* remaining prime factor */
    return tau;
}

/* Smallest prime factor of a 64‑bit integer (1 → 1) */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return 1;
    if (n % 2 == 0) return 2;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) return p;
    }
    return n;   /* n itself is prime */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t tau = divisor_count_u64((uint64_t)n);
    uint64_t spf = smallest_prime_factor(tau);
    return (seq_t)spf;
}
