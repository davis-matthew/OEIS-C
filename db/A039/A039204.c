/*
 * A039204  Numbers whose base-11 representation has the same number of 7's
 *          and 8's.
 * https://oeis.org/A039204
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

/* Numbers whose base-11 representation has the same number of 7's and 8's.
   The function scans non‑negative integers in order and returns the n‑th one
   (offset 1). */
seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t val = 0;
    while (found < n) {
        seq_t t = val;
        int c7 = 0, c8 = 0;
        if (t == 0) {
            /* representation "0": 0 7's and 0 8's */
        } else {
            while (t > 0) {
                seq_t d = t % (seq_t)11;
                t = t / (seq_t)11;
                if (d == 7) ++c7;
                else if (d == 8) ++c8;
            }
        }
        if (c7 == c8) {
            ++found;
            if (found == n) return val;
        }
        ++val;
    }
    /* unreachable but present to satisfy compiler */
    return val;
}
