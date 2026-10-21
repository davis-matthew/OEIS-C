/*
 * A057237  Maximum k <= n such that 1, 2, ..., k are all relatively prime to
 *          n.
 * https://oeis.org/A057237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (smallest prime dividing n) - 1 for n ≥ 2, a(1) = 1
 * Range:     u8 n <= 256, u16 n <= 9611, u32 n <= 9611, u64 n <= 9611,
 *            u128 n <= 9611, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the smallest prime factor of a 64‑bit unsigned integer.
   For n ≥ 2 this returns the least prime divisor; for prime n it returns n itself. */
static seq_t _smallest_prime_factor(uint64_t n)
{
    if (n % 2ULL == 0ULL)
        return (seq_t)2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL)
            return (seq_t)d;
    }
    return (seq_t)n; /* n is prime */
}

seq_t a(idx_t n)
{
    if (n <= 1)  /* a(1) == 1, and n is always ≥1 by the API contract */
        return (seq_t)1;
    /* For n ≥ 2, the answer is the smallest prime factor minus 1. */
    return _smallest_prime_factor((uint64_t)n) - 1;
}
