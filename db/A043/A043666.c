/*
 * A043666  Numbers whose base-14 representation has exactly 5 runs.
 * https://oeis.org/A043666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   this function searches for the n‑th integer whose base‑14
 *            representation has exactly five runs
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is the first such number. */
    if (n == 0) return 0;  /* undefined, return 0 */

    idx_t found = 0;
    seq_t X = 1;

    for (;;) {
        seq_t tmp = X;
        int prev = -1;
        int runs = 0;
        while (tmp > 0) {
            int d = (int)(tmp % 14);
            if (d != prev) {
                runs++;
                prev = d;
            }
            tmp /= 14;
        }
        if (runs == 5) {
            ++found;
            if (found == n)
                return X;
        }
        ++X;
    }
}
