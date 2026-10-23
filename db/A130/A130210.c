/*
 * A130210  Triangle read by rows: T(n, k) = A000005(k) if k divides n, T(n,
 *          k) = 0 otherwise.
 * https://oeis.org/A130210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = τ(k) if k | r else 0, where r is the smallest integer
 *            with r(r+1)/2 ≥ n, k = n – r(r–1)/2. τ(k) is the number of
 *            divisors of k
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

static inline __int128 triangular(idx_t x)
{
    return (__int128)x * (x + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n with binary search. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute column k = n - (r-1)*r/2. */
    idx_t prev = (idx_t)(triangular(r - 1));
    idx_t k = n - prev;          /* 1 <= k <= r */

    /* If k divides r, return divisor count of k; otherwise 0. */
    if (k == 0) return (seq_t)0; /* safety, should not happen */
    if (r % k == 0) {
        return (seq_t)divisor_count_u64((uint64_t)k);
    }
    return (seq_t)0;
}
