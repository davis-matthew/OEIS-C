/*
 * A133566  Triangle read by rows: (1,1,1,...) on the main diagonal and
 *          (0,1,0,1,...) on the subdiagonal.
 * https://oeis.org/A133566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 when n equals (k+1)(k+2)/2 for some integer k ≥ 0
 *            (triangular numbers); a(n) = 1 when n equals (k+1)(k+2)/2 - 1
 *            and (k-1) is odd; otherwise a(n) = 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1; undefined for n = 0 */

    /* Find the row r such that (r+1)(r+2)/2 ≥ n > r(r+1)/2. */
    idx_t low  = 0, high = 1;
    while ((unsigned __int128)(high + 1) * (high + 2) / 2 < n)
        high <<= 1;                /* exponential search for a high bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cum = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                                  /* row index (0‑based) */

    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2 + 1); /* first index of row r */
    idx_t j = n - start;                            /* position within the row (0 … r) */

    if (r == 0)               return 1;                 /* the single element in the first row */
    if (j == r)               return 1;                 /* main diagonal element */
    if (j == r - 1)           return (seq_t)((r - 1) & 1); /* subdiagonal element */
    return 0;                                         /* all other positions are 0 */
}
