/*
 * A090047  Length of longest contiguous block of 0's in binary expansion of
 *          n^2.
 * https://oeis.org/A090047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest contiguous block of 0's in the
 *            binary representation of n²
 * Range:     u8 n <= 15, u16 n <= 1699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n² in the correct integer type. */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Special case: 0^2 = 0 has a single binary digit “0”. */
    if (sq == 0) {
        return (seq_t)1;
    }

    /* Scan the bits of sq from least‑significant to most‑significant. */
    seq_t tmp = sq;
    unsigned max_run = 0;   /* longest zero run seen so far      */
    unsigned cur_run = 0;   /* current zero run length           */

    while (tmp > 0) {
        if ((tmp & (seq_t)1) == 0) {     /* current bit is 0 */
            ++cur_run;
            if (cur_run > max_run) {
                max_run = cur_run;
            }
        } else {                         /* current bit is 1 */
            cur_run = 0;
        }
        tmp >>= 1;                       /* move to next bit   */
    }

    return (seq_t)max_run;
}
