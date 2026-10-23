/*
 * A191669  Dispersion of A004767 (4k+3, k>=0), by antidiagonals.
 * https://oeis.org/A191669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is produced by filling an infinite array row‑wise. For each
 *            row i the first element a(i,1) is the i‑th positive integer that
 *            is NOT congruent to 3 (mod 4). All subsequent elements in the
 *            row satisfy the recurrence a(i,j+1) = 4·a(i,j) – 1 . The index n
 *            is mapped to coordinates (i,j) by traver...
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 153, u64 n <= 561,
 *            u128 n <= 2145, big any n
 * Helpers:   first_row_value, isqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt.h"
#include "../utils/pow_uint.h"
#include "../utils/first_row_value.h"

/* Convert 1‑based index n to antidiagonal coordinates (i,j). */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find the smallest d such that d*(d-1)/2 >= n.  */
    idx_t low = 2, high = 2;
    while (((unsigned __int128)high * (high - 1) / 2) < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid - 1) / 2) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;
    idx_t pre = (unsigned __int128)(d - 2) * (d - 1) / 2;
    *i = n - pre;          /* row  (1‑based)              */
    *j = d - *i;           /* column (1‑based)            */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n>=1 by definition */

    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    seq_t first = first_row_value(i);              /* a(i,1)   */
    seq_t pow4  = pow_uint(4, j - 1);              /* 4^(j-1)  */
    seq_t term  = first * pow4;                    /* first·4^(j-1)   */
    seq_t sub   = (pow4 - 1) / 3;                  /* (4^(j-1)-1)/3   */
    return term - sub;                             /* a(i,j)   */
}
