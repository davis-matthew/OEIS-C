/*
 * A166352  Table read by rows where row n contains the sorted digits of the
 *          integers 0 through n.
 * https://oeis.org/A166352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166352:
   Concatenate, for each n≥0, the sorted digits of all integers from
   0 through n.  The sequence is read from these rows in order. */

seq_t a(idx_t n)
{
    idx_t r = 0;
    seq_t counts[10] = {0};
    idx_t cumulative = 0; /* total digits before the current row */
    idx_t rowDigits = 0;   /* digits in numbers 0..r */

    for (;; ++r) {
        /* add the digits of the number r to the current row */
        idx_t x = r;
        if (x == 0) {
            counts[0]++; rowDigits++;
        } else {
            while (x > 0) {
                int d = x % 10;
                counts[d]++; rowDigits++;
                x /= 10;
            }
        }

        idx_t after = cumulative + rowDigits;
        if (n < after) {               /* target digit is in this row */
            idx_t offset = n - cumulative;
            for (int d = 0; d < 10; ++d) {
                seq_t c = counts[d];
                if ((seq_t)offset < c) {
                    return (seq_t)d;
                }
                offset -= (idx_t)c;
            }
            return (seq_t)0;           /* should never be reached */
        }

        cumulative = after;           /* move to the next row */
    }
}
