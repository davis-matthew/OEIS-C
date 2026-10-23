/*
 * A039146  Numbers whose base-10 representation has the same number of 5's
 *          and 6's.
 * https://oeis.org/A039146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑10 representation has the same number of 5’s and 6’s */
seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t val = 0;
    for (;;) {
        seq_t tmp = val;
        unsigned int cnt5 = 0, cnt6 = 0;
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            if (d == 5) ++cnt5;
            else if (d == 6) ++cnt6;
            tmp /= 10;
        }
        if (cnt5 == cnt6) {
            ++found;
            if (found == n) return val;
        }
        ++val;
    }
}
