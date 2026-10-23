/*
 * A045146  Numbers whose base-4 representation contains exactly two 2's and
 *          three 3's.
 * https://oeis.org/A045146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base-4 representation contains
 *            exactly two 2's and three 3's
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t value = 1;; value++) {
        seq_t v = value;
        int cnt2 = 0, cnt3 = 0;
        while (v > 0) {
            seq_t r = v % 4;
            if (r == 2) cnt2++;
            else if (r == 3) cnt3++;
            v /= 4;
        }
        if (cnt2 == 2 && cnt3 == 3) {
            if (++found == n) return value;
        }
    }
}
