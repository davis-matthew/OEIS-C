/*
 * A097806  Riordan array (1+x, 1) read by rows.
 * https://oeis.org/A097806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8066, u16 n <= 8066, u32 n <= 8066, u64 n <= 8066,
 *            u128 n <= 8066, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097806.
 *  a(0) = a(1) = a(2) = 1.
 *  For n ≥ 3 the sequence is built from successive blocks.
 *  Block k (k = 1,2,…) has k zeros followed by two ones,
 *  thus its length is k+2.  The blocks appear one after another.
 */

#include <stdint.h>  /* for consistency, can be omitted if idx_t/seq_t are provided elsewhere */

seq_t a(idx_t n)
{
    /* Initial terms */
    if (n <= 2)
        return (seq_t)1;

    /* Remaining position after the first three terms */
    idx_t pos = n - 3;
    idx_t k = 1;               /* current block number (k >= 1) */

    while (1) {
        idx_t block_len = k + 2;   /* length of block k */

        if (pos < block_len) {
            /* Inside the current block */
            if (pos < k)          /* first k positions are zeros */
                return (seq_t)0;
            else                  /* last two positions are ones */
                return (seq_t)1;
        }

        /* Skip this block and move to the next */
        pos -= block_len;
        ++k;
    }
}
