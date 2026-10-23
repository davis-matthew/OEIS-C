/*
 * A129602  In the binary expansion of n replace each run of k 0's (or 1's)
 *          with 2k-1 0's (or 1's), except in the most significant run where
 *          we double the number of 0's (or 1's).
 * https://oeis.org/A129602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary expansion of n with each run of k identical * bits
 *            replaced by 2k‑1 bits, except that the most significant run * is
 *            replaced by 2k bits
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* special case */
        return (seq_t)0;

    /* Determine the binary length of n (number of bits). */
    int bitlen = 0;
    for (idx_t t = n; t; ++bitlen)
        t >>= 1;

    /* Scan from most significant bit downwards, accumulating runs. */
    seq_t res = 0;
    int run_len = 1;
    int prev_bit = (int)((n >> (bitlen - 1)) & 1);
    int first_run = 1;                 /* true for the leading run */

    for (int i = bitlen - 2; i >= 0; --i) {
        int bit = (int)((n >> i) & 1);
        if (bit == prev_bit)
            ++run_len;
        else {
            /* Finish the current run. */
            idx_t newlen = first_run ? (run_len << 1)
                                     : ((run_len << 1) - 1);

            /* Build a mask of newlen ones. Avoid shifting by >= width. */
            idx_t width = sizeof(seq_t) * 8;
            seq_t mask;
            if (newlen >= width)
                mask = ~(seq_t)0;
            else
                mask = (((seq_t)1 << newlen) - 1);

            res = (res << newlen) | ((seq_t)prev_bit * mask);

            /* Prepare for next run. */
            first_run = 0;
            prev_bit = bit;
            run_len  = 1;
        }
    }

    /* Handle the final run. */
    idx_t newlen = first_run ? (run_len << 1)
                             : ((run_len << 1) - 1);
    idx_t width  = sizeof(seq_t) * 8;
    seq_t mask;
    if (newlen >= width)
        mask = ~(seq_t)0;
    else
        mask = (((seq_t)1 << newlen) - 1);

    res = (res << newlen) | ((seq_t)prev_bit * mask);

    return res;
}
