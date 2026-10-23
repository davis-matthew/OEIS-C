/*
 * A185740  Weight array of A185738, by antidiagonals.
 * https://oeis.org/A185740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 152, u32 n <= 560, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n): values from the infinite weight array with row 1 all 1 s and
 * column 1 2^(i–1).  Entries are enumerated by antidiagonals:
 * diagonal 1 → (1,1)
 * diagonal 2 → (1,2),(2,1)
 * diagonal 3 → (1,3),(2,2),(3,1)
 * … 
 * For a given n the coordinates (i,j) of the nth element are found,
 * then:
 *     if i==1          →  1
 *     else if j==1     →  2^(i-1)
 *     else             →  0
 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* offset is 1; undefined for n==0 */

    /* Find the smallest d such that d*(d+1)/2 ≥ n  (d = diagonal index). */
    idx_t low  = 1;
    idx_t high = 2 * n;                      /* γ · n is an upper bound */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = mid * (mid + 1) / 2;     /* triangular number of mid */
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;

    /* Position inside the diagonal (1‑based). */
    idx_t prev = (d - 1) * d / 2;
    idx_t pos  = n - prev;                   /* 1‑based index on diagonal */
    idx_t i    = pos;                        /* row index (1‑based) */
    idx_t j    = d + 1 - pos;                /* column index (1‑based) */

    if (i == 1)
        return (seq_t)1;                     /* row 1 all 1 s */
    if (j == 1)
        return pow2(i - 1);                  /* column 1, powers of 2 */
    return (seq_t)0;                         /* all other entries are 0 */
}
