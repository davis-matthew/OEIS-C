/*
 * A134560  Triangle A051731 * A127775 (as infinite lower triangular
 *          matrices).
 * https://oeis.org/A134560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (row r, column c s.t. r%{c}=0 ? 2c-1 : 0) where r is the
 *            smallest integer with r(r+1)/2 >= n and c = n - (r-1)r/2. This
 *            is the flattening of the triangle A051731 with 1→2c-1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* _floor_isqrt_u64 and other helpers may be available, but we use
   binary search with 128‑bit arithmetic to avoid overflow. */

seq_t a(idx_t n)
{
    /* n >= 1 for meaningful values; return 0 otherwise. */
    if (n == 0) return (seq_t)0;

    /* Find r such that r(r+1)/2 >= n, using binary search. */
    idx_t low = 1;
    idx_t high = 1;
    /* Expand high until it satisfies the triangular condition. */
    while (1) {
        __uint128_t tri = (__uint128_t)high * (high + 1) / 2;
        if (tri >= n) break;
        /* Prevent overflow when doubling high. */
        if (high > (UINT64_MAX >> 1)) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* Compute column index c. */
    __uint128_t prev_tri = (__uint128_t)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev_tri;

    /* Return 2c-1 if r is divisible by c, otherwise 0. */
    if (r % c == 0) {
        return (seq_t)((seq_t)2 * (seq_t)c - 1);
    } else {
        return (seq_t)0;
    }
}
