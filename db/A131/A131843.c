/*
 * A131843  Triangle read by rows: 2*A131821 - A000012.
 * https://oeis.org/A131843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 8255, u16 n <= 9326, u32 n <= 9326, u64 n <= 9326,
 *            u128 n <= 9326, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) for OEIS A131843: the flattened lower‑triangular
 * array where row r (0‑based) has length r+1, border entries equal
 * to 2r+1 and all interior entries equal to 1.
 *
 * a(n) = 2r+1  if n - r(r+1)/2 == 0  or  n - r(r+1)/2 == r
 *        1      otherwise
 *
 * The function works for any idx_t ≥ 0 and uses only O(log n)
 * arithmetic with integer types.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.  */
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand high until its triangular number exceeds n. */
    while (1) {
        __int128 s = (__int128)high * (high + 1) / 2;
        if (s > n) break;
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 s = (__int128)mid * (mid + 1) / 2;
        if (s <= n) low = mid;
        else       high = mid;
    }

    idx_t r = low;                        /* row index */
    idx_t start = (idx_t)((__int128)r * (r + 1) / 2);  /* first index of row r */
    idx_t c = n - start;                  /* column index in row r */

    if (c == 0 || c == r) {
        return (seq_t)(2 * r + 1);
    } else {
        return (seq_t)1;
    }
}
