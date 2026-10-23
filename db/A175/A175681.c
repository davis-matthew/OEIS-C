/*
 * A175681  The sum of maximal run and minimal run lengths in binary
 *          representation of n.
 * https://oeis.org/A175681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of maximal and minimal run lengths in binary
 *            representation of n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n >= 1 as guaranteed by the sequence offset. */
    /* Determine the position of the most significant 1-bit. */
    idx_t msb_pos = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        msb_pos++;
    }

    seq_t max_run = 0;
    seq_t min_run = 0;
    seq_t curr_run = 0;
    int prev_bit = -1;

    for (idx_t pos = msb_pos; ; pos--) {
        int bit = (int)((n >> pos) & 1);

        if (pos == msb_pos) {          /* first (most significant) bit */
            prev_bit = bit;
            curr_run = 1;
        } else {
            if (bit == prev_bit) {
                curr_run++;
            } else {                  /* run ended */
                if (max_run == 0) {
                    max_run = curr_run;
                    min_run = curr_run;
                } else {
                    if (curr_run > max_run) max_run = curr_run;
                    if (curr_run < min_run) min_run = curr_run;
                }
                prev_bit = bit;
                curr_run = 1;
            }
        }

        if (pos == 0) break;           /* finished processing LSB */
    }

    /* Record the final run (which may have been the only run). */
    if (max_run == 0) {
        max_run = curr_run;
        min_run = curr_run;
    } else {
        if (curr_run > max_run) max_run = curr_run;
        if (curr_run < min_run) min_run = curr_run;
    }

    return max_run + min_run;
}
