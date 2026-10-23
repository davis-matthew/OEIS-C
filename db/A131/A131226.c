/*
 * A131226  Triangle read by rows: T(n,k) = 3*k - 1 - (-1)^(n+k), for 1 <= k
 *          <= n, n >= 1.
 * https://oeis.org/A131226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = 3*k - 1 - (-1)^(n+k)
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained by locating the flattened index n in the triangle,
   determining the row r and column k, and evaluating the formula. */

#include <stdint.h>
#include <stddef.h>

/* Find the row index r such that r*(r+1)/2 >= n (1‑based indexing). */
static inline idx_t
row_for_n(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Expand high until the triangular number exceeds n. */
    while ( ((unsigned __int128)high * (high + 1)) / 2 < n ) {
        high <<= 1;
        if (high == 0) {          /* protect against idx_t overflow */
            high = (idx_t)(-1) >> 1;
            break;
        }
    }
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ( ((unsigned __int128)mid * (mid + 1)) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* behavior for indices < 1 is unimportant */
        return 0;

    idx_t r = row_for_n(n);                        /* row number (1‑based) */
    idx_t prev = ((unsigned __int128)(r - 1) * r) / 2; /* elements before row r */
    idx_t k = n - prev;                            /* column within the row (1‑based) */

    idx_t sum = r + k;        /* parity of exponent in the formula */
    seq_t val3k = (seq_t)3 * (seq_t)k;  /* 3*k fits into seq_t for the value range */

    return ((sum & 1) == 0) ? (val3k - 2) : val3k;
}
