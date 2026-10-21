/*
 * A082523  Number of times k^2 + (n-k)^2 is a square for 1 <= k <= n-1.
 * https://oeis.org/A082523
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k (1 ≤ k < n) for which k² + (n−k)² is a
 *            perfect square
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values using binary search and 128‑bit arithmetic */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2, but the implementation
       returns 0 for smaller indices without a crash. */
    if (n < 2)
        return (seq_t)0;

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t k2 = k * k;
        idx_t mk = n - k;
        idx_t mk2 = mk * mk;
        idx_t sum = k2 + mk2;               /* ≤ 2 n², fits in idx_t for n < 2¹⁰⁴ */
        idx_t r = isqrt_u64(sum);
        if (r * r == sum)
            ++count;
    }
    return count;
}
