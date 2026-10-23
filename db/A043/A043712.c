/*
 * A043712  Numbers whose base-9 representation has an odd number of runs.
 * https://oeis.org/A043712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer whose base‑9 representation has an odd
   number of runs (consecutive equal digits).  */

seq_t a(idx_t n)
{
    seq_t found = 0;                 /* count of numbers with odd runs */
    for (idx_t num = 1; ; ++num)    /* iterate all positive integers */
    {
        /* Count runs in base‑9 representation of num */
        int last_digit = -1;
        int runs = 0;
        idx_t t = num;
        while (t > 0) {
            int d = t % 9;
            t    /= 9;
            if (runs == 0 || d != last_digit) {
                ++runs;
                last_digit = d;
            }
        }
        if (runs & 1) {              /* odd number of runs */
            ++found;
            if (found == n)
                return (seq_t)num;
        }
    }
}
