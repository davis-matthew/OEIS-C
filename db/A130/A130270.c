/*
 * A130270  Triangle read by rows, T(n) followed by 1, 2, 3, ..., n-1.
 * https://oeis.org/A130270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130270: triangle read by rows; row r starts with
 * the triangular number T(r) = r*(r+1)/2 and is then followed by
 * 1,2,…,r‑1.
 * For a given index n (n ≥ 1) we find the row r that contains n.
 * Let m = n‑1.  Then r = 1 + ⌊(√(8m+1) – 1)/2⌋.
 * The first index of row r is offset(r) = (r‑1)·r/2 + 1.
 *      a(n) = T(r)                if n == offset(r)
 *      a(n) = n – offset(r)       otherwise
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer
 * (floor of the square root).  No overflow in the multiplication
 * inside the comparison by using division. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = 1; continue; }
        if (mid <= x / mid)      /* mid*mid <= x  (avoids overflow) */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Determine the row r that contains index n */
    idx_t m   = n - 1;                     /* m = n-1 >= 0 */
    idx_t t   = 8 * m + 1;                 /* 8*m + 1 */
    idx_t root = isqrt_u64(t);             /* floor(sqrt(8m+1)) */
    idx_t k   = (root - 1) / 2;            /* k = floor((√(8m+1)-1)/2) */
    idx_t r   = k + 1;                     /* row number, r ≥ 1 */

    /* offset of the first element of row r */
    seq_t TriK = (seq_t)k * (k + 1) / 2;   /* T(k) = (r-1)·r/2 */
    seq_t offset = TriK + 1;               /* first index of row r */

    if ((seq_t)n == offset) {             /* first element of the row */
        return (seq_t)r * (r + 1) / 2;     /* T(r) */
    } else {                              /* the sequence 1,2,…,r-1 */
        return (seq_t)n - offset;
    }
}
