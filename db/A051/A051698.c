/*
 * A051698  Closest prime to n that is different from n (break ties by taking
 *          the smaller prime).
 * https://oeis.org/A051698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = closest prime to n that is different from n; * ties are
 *            broken by choosing the smaller prime
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches outward from n, checking progressively
 * larger offsets until a suitable prime is found. Prime testing is
 * done by trial division using the 6k±1 optimisation.  It is fast
 * for n up to about 10 000 and can serve as a generic routine for
 * the 64‑bit build. */
#include <stdint.h>

static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    if (n % 3ULL == 0ULL) return n == 3ULL;
    for (uint64_t i = 5ULL; i <= n / i; i += 6ULL) {
        if (n % i == 0ULL || n % (i + 2ULL) == 0ULL) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    /* For very small n, the answer is always 2. */
    if (n < 2U) return 2;

    /* Search outward from n until a prime different from n is found.
     * At each offset we test lower first, then upper, so ties
     * automatically favour the smaller prime. */
    for (uint64_t offset = 1ULL; ; ++offset) {
        /* lower candidate, skip if it would underflow below 2 */
        if (n >= offset) {
            uint64_t low = n - offset;
            if (low >= 2U && is_prime_uint64(low))
                return (seq_t)low;
        }
        /* upper candidate */
        uint64_t high = n + offset;
        if (is_prime_uint64(high))
            return (seq_t)high;
    }
}
