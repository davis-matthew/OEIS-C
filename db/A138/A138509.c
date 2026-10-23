/*
 * A138509  Janet (or left-step) periodic table from right to left. 120 terms.
 *          8 rows, 32 columns without spaces.
 * https://oeis.org/A138509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is obtained by reading descending sequences of lengths *
 *            2,2,8,8,18,18,32,32,… (that is, two blocks of size 2k² for
 *            k=1,2,3,…). * For each block the values are C_end, C_end-1, …,
 *            C_end-size+1, * where C_end is the cumulative total after that
 *            block. * The function computes the block index and...
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   cum_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cum_block.h"

seq_t a(idx_t n) {
    if (n < 1) return 0; /* Offset starts at 1; undefined for n==0 but guard */

    /* Find block pair index p such that cum_block(p) >= n > cum_block(p-1) */
    uint64_t p = 1;
    seq_t cum = cum_block(p);
    while (cum < n) {
        ++p;
        cum = cum_block(p);
    }

    seq_t cum_prev = (p > 1) ? cum_block(p - 1) : 0;          /* D(p-1) */
    seq_t size = 2 * (seq_t)p * (seq_t)p;                    /* block size in this pair */
    seq_t end_first = cum_prev + size;                       /* end index of first block */

    if (n <= end_first) {
        /* First block of the pair */
        seq_t offset = (seq_t)n - (cum_prev + 1);            /* zero‑based offset */
        return end_first - offset;
    } else {
        /* Second block of the pair */
        seq_t offset = (seq_t)n - (end_first + 1);           /* zero‑based offset */
        seq_t end_second = cum;                              /* D(p) */
        return end_second - offset;
    }
}
