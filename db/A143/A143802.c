/*
 * A143802  Triangle read by rows, "n" followed by (n-1) terms of (1, 3, 7,
 *          15, ...).
 * https://oeis.org/A143802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2_minus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143802:  Triangle read by rows
 * Row r:   r, 1, 3, 7, 15, ...  (2^k‑1 for k = 1..r‑1)
 * a(n) is obtained by locating the row r where r(r+1)/2 ≥ n,
 * then returning r if the element is the first in the row,
 * otherwise 2^k−1 where k = n – (r−1)r/2 – 1 (the offset within the row).
 */
#include <stddef.h>
#include "../utils/pow2_minus1.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for out‑of‑range indices */

    idx_t r   = 1;   /* current row number */
    idx_t sum = 0;   /* total elements before row r */

    /* find the row containing position n */
    while (sum + r < n) {
        sum += r;
        ++r;
    }

    idx_t offset = n - sum - 1; /* 0 for the first element of the row */

    if (offset == 0)
        return (seq_t)r;          /* the leading element of the row */
    else
        return pow2_minus1(offset); /* 2^k−1 term */
}
