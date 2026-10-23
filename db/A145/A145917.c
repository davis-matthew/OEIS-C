/*
 * A145917  Triangle read by rows: to get n-th row, start with -4n and
 *          successively add 5, 7, 9, 11, 13, ... until reaching a square.
 * https://oeis.org/A145917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = -4*row + k(k+4), where * row = floor((sqrt(8n+1)-1)/2) *
 *            k = n - row(row+1)/2 * This is equivalent to the OEIS
 *            definition: * start at -4*row and add successive odd numbers
 *            5,7,9,… until a square is reached
 * Range:     i8 n <= 89, i16 n <= 9869, i32 n <= 9869, i64 n <= 9869,
 *            i128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute triangular number r(r+1)/2 safely in 128‑bit */
static inline __int128 tri128(uint64_t r)
{
    return (__int128)r * (r + 1) / 2;
}

/* Helper: compare if tri(r) <= n  (using 128‑bit to avoid overflow) */
static inline bool tri_le(uint64_t r, uint64_t n)
{
    return tri128(r) <= (__int128)n;
}

/* Find the row index for a given linear index n.
 * Returns the largest r with triangular(r) <= n.
 */
static uint64_t find_row(uint64_t n)
{
    uint64_t low = 0, high = 1;
    while (tri_le(high, n)) {
        /* avoid infinite loop when high overflows */
        if (high > (UINT64_MAX >> 1)) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if (tri_le(mid, n))
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* step 1: row index */
    uint64_t r = find_row((uint64_t)n);

    /* step 2: offset k within the row */
    uint64_t tri = (uint64_t)tri128(r); /* r(r+1)/2 fits in 64 bits here */
    uint64_t k = (uint64_t)n - tri;

    /* step 3: compute the value using the explicit formula
     * val = -4*r + k(k+4)  -> cast to seq_t to preserve sign and size
     */
    seq_t val = (seq_t)k * ((seq_t)k + (seq_t)4) - (seq_t)4 * (seq_t)r;
    return val;
}
