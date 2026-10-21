/*
 * A217575  Numbers n such that floor(sqrt(n)) = floor(n/floor(sqrt(n)))-1.
 * https://oeis.org/A217575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k*(k+1) + i - 1 where k = ceil( (-1 + sqrt(1+8n))/2 ) and
 *            i = n - k*(k-1)/2 (1 ≤ i ≤ k). This produces the sequence of all
 *            n such that floor(sqrt(n)) = floor(n/floor(sqrt(n))) - 1
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute floor(sqrt(x)) for 64‑bit unsigned integers using binary search.
   The result fits in idx_t, which is an unsigned 64‑bit type. */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid underflow */
            high = mid - 1;
        }
    }
    return high;
}

/* Return the n-th term of OEIS A217575. */
seq_t a(idx_t n)
{
    /* Find k such that k*(k+1)/2 ≥ n and (k-1)*k/2 < n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 lhs = (__int128)mid * (mid + 1) / 2;
        if (lhs >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t k = low;

    /* Compute i = n - (k-1)*k/2. */
    __int128 prev_sum = (__int128)(k - 1) * k / 2;
    idx_t i = n - (idx_t)prev_sum;

    /* Convert to seq_t for the final computation. */
    seq_t sk = (seq_t)k;
    seq_t si = (seq_t)i;
    seq_t result = sk * (sk + 1) + si - 1;  /* a(n) = k*(k+1) + i - 1 */
    return result;
}
