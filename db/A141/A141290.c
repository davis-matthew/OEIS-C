/*
 * A141290  Triangle read by rows, descending antidiagonals of a (1, 3, 5,
 *          ...) * (1, 4, 16, ...) multiplication table.
 * https://oeis.org/A141290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*i-1) * 4^(j-1) where (i,j) is the index in the *
 *            multiplication table of odd numbers (columns) and powers of 4
 *            (rows), * read by descending antidiagonals: for each s = i+j
 *            (starting at 2), * list (i=j? no) entries with i decreasing from
 *            s‑1 to 1
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by: a(1)=1, a(2)=3, a(3)=4, ... as given
 * in OEIS A141290. This implementation derives i and j from n
 * by binary searching the antidiagonal index.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow4.h"  /* Provides seq_t pow4(idx_t exp) */

seq_t a(idx_t n)
{
    /* Find the smallest S such that (S-1)*S/2 >= n.
     * (S-1)*S/2 is the number of entries up to the antidiagonal
     * with sum S. We use binary search with 128‑bit arithmetic
     * to avoid overflow. */
    idx_t low = 1, high = 2;
    while ((unsigned __int128)(high - 1) * high / 2 < n && high < UINT64_MAX / 2)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)(mid - 1) * mid / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t S = low;                      /* current antidiagonal sum */
    idx_t prev = (S >= 2) ? ((unsigned __int128)(S - 2) * (S - 1) / 2) : 0;
    idx_t d = n - prev - 1;             /* offset inside antidiagonal (0‑based) */
    idx_t i = S - 1 - d;                /* row index (column heading index) */
    /* j = 1 + d; */                   /* column index (power of 4 index) */

    /* Compute (2*i-1) * 4^(j-1).  All arithmetic in seq_t. */
    seq_t odd = (seq_t)(2 * i - 1);     /* odd column multiplier */
    seq_t pow = pow4(d);                /* 4^(j-1) where d = j-1 */
    return odd * pow;
}
