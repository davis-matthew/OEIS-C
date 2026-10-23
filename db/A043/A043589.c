/*
 * A043589  Numbers whose base-3 representation has exactly 9 runs.
 * https://oeis.org/A043589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of numbers whose base‑3 expansion has exactly 9 runs.
   It simply scans integers in increasing order, counts the runs, and returns
   the n‑th such integer. */

seq_t a(idx_t n)
{
    idx_t found = 0;      /* number of terms found so far            */
    for (idx_t val = 1; ; ++val) {
        seq_t x = (seq_t)val;      /* work with the value as seq_t      */
        /* Count the runs in the base‑3 representation of x.
           Iterate from least significant digit to most significant. */
        idx_t prev = (idx_t)(x % 3);   /* first digit (least significant) */
        idx_t runs = 1;
        x /= 3;
        while (x > 0) {
            idx_t d = (idx_t)(x % 3);
            if (d != prev) {
                ++runs;
                prev = d;
            }
            x /= 3;
        }
        if (runs == 9) {
            ++found;
            if (found == n)
                return (seq_t)val;
        }
    }
}
