/*
 * A039046  Numbers whose base-7 representation has the same number of 3's and
 *          6's.
 * https://oeis.org/A039046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-7 representation has the same number of 3's and 6's
   (OEIS A039046).  Generating by brute‑force scan. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t value = 0;
    for (;; ++value) {
        seq_t tmp = value;
        idx_t count3 = 0, count6 = 0;
        if (tmp == 0) {
            /* 0 has zero 3's and zero 6's – it meets the requirement. */
        } else {
            while (tmp != 0) {
                seq_t digit = tmp % 7;
                if (digit == 3) ++count3;
                else if (digit == 6) ++count6;
                tmp = tmp / 7;
            }
        }
        if (count3 == count6) {
            ++found;
            if (found == n) return value;
        }
    }
}
