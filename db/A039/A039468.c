/*
 * A039468  Numbers whose base-11 representation has the same nonzero number
 *          of 3's and 7's.
 * https://oeis.org/A039468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039468: Numbers whose base-11 representation has the same nonzero number of 3's and 7's */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t cand = 1; ; cand++) {
        seq_t x = (seq_t)cand;
        int cnt3 = 0, cnt7 = 0;
        while (x != 0) {
            int digit = (int)(x % 11);
            if (digit == 3) cnt3++;
            else if (digit == 7) cnt7++;
            x /= 11;
        }
        if (cnt3 == cnt7 && cnt3 > 0) {
            found++;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
