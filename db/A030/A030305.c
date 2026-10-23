/*
 * A030305  Length of n-th run of 0's in A030302.
 * https://oeis.org/A030305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1010, u16 n <= 1010, u32 n <= 1010, u64 n <= 1010,
 *            u128 n <= 1010, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030305: length of the n‑th run of zeros in the binary
   concatenation of the positive integers (1,2,3,…) written without
   leading zeros. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* target run number */
    idx_t target      = n;       /* n ≥ 1 (offset is 1) */
    idx_t run_counter = 0;       /* runs found so far */
    seq_t cur_run_len = 0;       /* length of the current zero run */
    int prev_bit      = -1;      /* previous bit (-1 = start of stream) */

    for (idx_t num = 1; ; ++num) {
        /* find most significant bit position of num */
        idx_t t   = num;
        idx_t msb = 0;
        t >>= 1;
        while (t) {
            ++msb;
            t >>= 1;
        }

        /* process bits from most significant to least significant */
        for (idx_t i = msb + 1; i > 0; --i) {
            int bit = (int)((num >> (i - 1)) & 1);

            if (bit == 0) {
                if (prev_bit == 0)
                    ++cur_run_len;          /* extend current run */
                else
                    cur_run_len = 1;         /* start new run */
            } else {                         /* bit == 1 */
                if (prev_bit == 0) {         /* zero run just finished */
                    ++run_counter;
                    if (run_counter == target)
                        return cur_run_len;   /* answer found */
                    cur_run_len = 0;        /* reset for next run */
                }
            }
            prev_bit = bit;
        }
    }
}
