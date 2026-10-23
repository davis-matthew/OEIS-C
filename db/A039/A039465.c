/*
 * A039465  Numbers whose base-11 representation has the same nonzero number
 *          of 3's and 4's.
 * https://oeis.org/A039465
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

/* Computes numbers whose base‑11 representation has the same non‑zero
   number of 3’s and 4’s.  The sequence is enumerated by testing
   successive integers in increasing order. */

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1, input 0 is undefined */
    seq_t count = 0;
    for (seq_t num = 1; ; ++num) {     /* iterate over all positive integers */
        seq_t tmp = num;
        int c3 = 0, c4 = 0;
        while (tmp > 0) {
            int d = (int)(tmp % 11);   /* digit in base 11 */
            if (d == 3) ++c3;
            else if (d == 4) ++c4;
            tmp /= 11;
        }
        if (c3 > 0 && c3 == c4) {      /* both digits appear equally often */
            ++count;
            if (count == n) return num;
        }
    }
}
