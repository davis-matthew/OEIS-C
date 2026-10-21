/*
 * A230445  Triangle read by rows: T(n,m) = 3^m*2^(n-m)-1, the number of
 *          neighbors in an n-dimensional cubic array.
 * https://oeis.org/A230445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 3^m * 2^(r-m) – 1 where r = floor((sqrt(1+8*n)–1)/2) and
 *            m = n – r*(r+1)/2
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Compute the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
   Uses a binary search with 128‑bit intermediate arithmetic to avoid
   overflow. */
static inline idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow upper bound until it exceeds n */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n)
        high <<= 1;

    /* Binary search between [low, high) */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    return low;               /* r = low */
}

seq_t a(idx_t n)
{
    idx_t r   = find_row(n);                     /* row index 0‑based */
    unsigned __int128 tri = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t    m   = n - (idx_t)tri;               /* position in the row */

    seq_t val = pow3(m);                         /* 3^m */
    val *= pow2(r - m);                          /* * 2^(r-m) */
    val -= (seq_t)1;                             /* - 1 */

    return val;
}
