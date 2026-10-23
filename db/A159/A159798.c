/*
 * A159798  Triangle read by rows in which row n lists n terms, starting with
 *          1, such that the difference between successive terms is equal to
 *          n-3.
 * https://oeis.org/A159798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(n,k) = 1 + k*(n-3), 0 ≤ k < n
 * Range:     u8 n <= 170, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 value (floor). */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1) << 35;  /* 2^35 > sqrt(2^67) */
    uint64_t ans = 0;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Find the row number r for a given index n (1‑based). */
static inline idx_t find_row(idx_t n)
{
    /* Compute r0 = floor((-1 + floor_sqrt(1+8*n))/2). */
    unsigned __int128 xx = (unsigned __int128)1 + (unsigned __int128)8 * n;
    uint64_t s = isqrt_u128(xx);
    uint64_t r0 = (s - 1) / 2;

    unsigned __int128 cumul = (__int128)r0 * (r0 + 1) / 2;
    if (cumul < n) ++r0;
    return (idx_t)r0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, any value ok */

    idx_t row = find_row(n);

    /* k = position inside the row (0‑based). */
    unsigned __int128 prev = (__int128)(row - 1) * row / 2;  /* cumulative before this row */
    idx_t k = n - (idx_t)prev - 1;

    /* Handle the first two rows explicitly (negative differences). */
    if (row == 1) return (seq_t)1;
    if (row == 2) return (k == 0) ? (seq_t)1 : (seq_t)0;

    /* For row ≥ 3 the difference (row−3) is non‑negative. */
    idx_t diff = row - 3;
    seq_t term = (seq_t)1 + (seq_t)k * (seq_t)diff;
    return term;
}
