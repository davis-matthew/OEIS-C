/*
 * A165191  Irregular triangle B(n,i) = i-th significant bit of Gray code of
 *          n.
 * https://oeis.org/A165191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A165191 :
   Row r consists of the reversed binary representation of the Gray
   code g(r) = r ^ (r >> 1).  The sequence is the flattening of all
   rows.  For a given index n, the function finds the row that
   contains n and returns the corresponding bit (0 or 1). */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t cum    = 0;   /* cumulative length of rows processed */
    idx_t row    = 0;   /* current row index */

    while (1) {
        /* Gray code of the current row */
        idx_t g = row ^ (row >> 1);

        /* Length of this row:
           – Row 0 has length 1 (bit “0”).
           – For other rows, g > 0, the number of bits equals
             bitlen_seq_t(g).  bitlen_seq_t returns
             floor(log2(g)) + 1. */
        idx_t len = (row == 0) ? (idx_t)1
                                : (idx_t)bitlen_seq_t((seq_t)g);

        /* Does the target index lie inside this row? */
        if (cum + len > target)
            break;

        /* Skip this row and continue with the next one */
        cum += len;
        ++row;
    }

    /* k is the 0‑based position inside the current row */
    idx_t k = target - cum;

    /* The row’s bits are the Gray code reversed; the leftmost
       bit of the reversed string corresponds to the least
       significant bit of the Gray code.  So the desired bit
       is simply bit k of g. */
    idx_t g = row ^ (row >> 1);
    int bit = (int)((g >> k) & 1ULL);

    return (seq_t)bit;
}
