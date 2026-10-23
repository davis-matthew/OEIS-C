/*
 * A121997  Count up to n, n times.
 * https://oeis.org/A121997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A121997: Count up to n, n times
   The n-th block consists of n subblocks, each of which counts from 1 to n.
   Let T(k) = 1^2 + 2^2 + … + k^2 = k(k+1)(2k+1)/6.
   Then the smallest k with T(k) ≥ n gives the current block.
   Within that block let offset = n – T(k‑1) – 1 (zero‑based).
   The value is (offset mod k) + 1.  */
#include <stdint.h>
static inline idx_t sum_sq(idx_t k) {
    return (k * (k + 1) * (2 * k + 1)) / 6;
}
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    idx_t low = 1, high = 1;
    /* Find an upper bound for k */
    while (sum_sq(high) < n) {
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    /* Binary search for the minimal k with sum_sq(k) ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (sum_sq(mid) >= n) high = mid;
        else low = mid + 1;
    }
    idx_t k = low;
    idx_t t_prev = (k > 1) ? sum_sq(k - 1) : (idx_t)0;
    idx_t offset = n - t_prev - 1;
    idx_t r = offset % k;
    return (seq_t)(r + 1);
}
