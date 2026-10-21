/*
 * A090001  Length of longest contiguous block of 1's in binary expansion of
 *          n^2.
 * https://oeis.org/A090001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 313, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the longest run of consecutive 1's in the binary representation of n^2. */
seq_t a(idx_t n)
{
    seq_t square = (seq_t)n * (seq_t)n;  /* compute n^2 */
    seq_t max_run = 0;
    seq_t curr = 0;

    while (square) {
        if (square & 1) {          /* bit is 1 */
            curr++;
            if (curr > max_run)
                max_run = curr;
        } else {                    /* bit is 0 */
            curr = 0;
        }
        square >>= 1;
    }
    return max_run;
}
