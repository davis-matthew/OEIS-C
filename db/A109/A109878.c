/*
 * A109878  Triangle read by rows: see below.
 * https://oeis.org/A109878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry of the triangular array whose row r (r ≥ 1)
 *            contains r terms, each term being the product of r‑2 consecutive
 *            integers taken modulo r. The index n (1‑based) determines the
 *            row r and the column c (1‑based within the row) by r = smallest
 *            integer with r(r+1)/2 ≥ n // row containing...
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the row r that contains the index n by binary search on
       triangular numbers T(r) = r(r+1)/2.  The values are small enough
       that 64‑bit arithmetic with 128‑bit intermediates suffices.      */
    idx_t lo = 1, hi = 1ULL << 32;                 /* hi > possible r */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Index of the first element of row r, 1-based. */
    idx_t first = (r - 1) * r / 2;                 /* fits in 64 bits */
    idx_t c = n - first;                            /* column, 1‑based */

    /* Compute the product of r‑2 consecutive numbers modulo r.  For r≤2
       the product is defined to be 1. */
    if (r <= 2)
        return (seq_t)1;

    seq_t product = 1;
    idx_t t = c - 1;                               /* 0‑based offset */
    for (idx_t i = 0; i < r - 2; ++i) {
        idx_t num = (t + i) % r + 1;                /* range 1 … r */
        product *= (seq_t)num;
    }
    return product;
}
