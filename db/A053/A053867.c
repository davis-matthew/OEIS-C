/*
 * A053867  Parity of sum of divisors of n less than n.
 * https://oeis.org/A053867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is an odd square, * 1 if n is an even square or
 *            twice a square, * otherwise n mod 2
 * Range:     u8 n <= 9585, u16 n <= 9585, u32 n <= 9585, u64 n <= 9585,
 *            u128 n <= 9585, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root (floor) for 64‑bit unsigned integers */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 1ULL << 32; /* sqrt(2^64‑1) < 2^32 */
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        /* divide to avoid overflow */
        if (mid > 0 && mid > n / mid) high = mid;
        else low = mid + 1;
    }
    return low - 1;
}

/* OEIS A053867: parity of sum of proper divisors of n. */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t r = isqrt_u64(nn);

    /* check for perfect square */
    if (r * r == nn) {
        return (seq_t)((r & 1ULL) ? 0 : 1);   /* odd square → 0, even square → 1 */
    }

    /* check if n is twice a square */
    if ((nn & 1ULL) == 0) {
        uint64_t half = nn >> 1;
        uint64_t s = isqrt_u64(half);
        if (s * s == half) return (seq_t)1;
    }

    /* otherwise parity is n mod 2 */
    return (seq_t)(nn & 1ULL);
}
