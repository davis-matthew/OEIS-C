/*
 * A106448  Table of (x+y)/gcd(x,y) where (x,y) runs through the pairs (1,1),
 *          (1,2), (2,1), (1,3), (2,2), (3,1), ...
 * https://oeis.org/A106448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (row + 1)/gcd(x, y) where the (x, y) pair is obtained by
 *            reading the triangle (1,1); (1,2),(2,1); (1,3),(2,2),(3,1); … by
 *            rows. The row of index n satisfies row*(row+1)/2 ≥ n
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t needed_row(idx_t n)
/* Find the row r such that   r*(r+1)/2 ≥ n   (r ≥ 1). */
{
    idx_t low = 1, high = 1;
    /* Find an upper bound for r */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* Binary search for the smallest r with r*(r+1)/2 ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

static inline seq_t seq_gcd(seq_t a, seq_t b)
/* Euclidean algorithm working for any integer type seq_t. */
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the row and position within the row. */
    idx_t r = needed_row(n);
    idx_t prev = (unsigned __int128)(r - 1) * r / 2; /* terms before this row */
    idx_t pos = n - prev;                           /* 1‑based position in row r */

    /* The pair (x, y) at this position. */
    idx_t x = pos;
    idx_t y = r - pos + 1;

    /* Compute (x + y)/gcd(x, y) = (r + 1)/gcd(x, y). */
    seq_t g = seq_gcd((seq_t)x, (seq_t)y);
    seq_t result = ((seq_t)r + (seq_t)1) / g;
    return result;
}
