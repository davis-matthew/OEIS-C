/*
 * A152906  Irregular triangle read by rows, numbers in A007318 repeated three
 *          times .
 * https://oeis.org/A152906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) equals the binomial coefficient C(r, k) where r is the row
 *            index in an irregular triangle of length 3*(r+1) rows that list
 *            the binomial coefficients of r repeated three times. The block
 *            index k is obtained by dividing the position within the row by 3
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    idx_t r   = 0;           /* current row index          */
    idx_t off = 0;           /* linear offset of current row */

    /* find the row that contains position n */
    while (1) {
        idx_t row_len = 3 * (r + 1);   /* number of entries in this row */
        if (off + row_len > n)
            break;
        off += row_len;
        ++r;
    }

    /* position inside the row: 0 .. 3*(r+1)-1   */
    idx_t pos = n - off;
    /* block (each block holds 3 identical entries)  */
    idx_t k = pos / 3;

    return binom_n_k(r, k);   /* C(r, k) */
}
