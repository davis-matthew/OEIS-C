/*
 * A087411  Multiples of 9 with digits grouped in pairs and leading zeros
 *          omitted.
 * https://oeis.org/A087411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A087411: concatenated multiples of 9 are broken into pairs of
   digits (most significant first); each pair is interpreted as a decimal
   number with leading zeros omitted.  The n-th such pair is returned. */

seq_t a(idx_t n) {
    idx_t pairs_needed = n;
    idx_t m = 9;          /* current multiple of 9 */
    int carry = -1;       /* first digit of the current pair (-1 if none) */

    while (1) {
        /* extract decimal digits of m into dig[], most significant first */
        int dig[20];
        int len = 0;
        idx_t t = m;
        if (t == 0) {
            dig[len++] = 0;
        } else {
            while (t > 0) {
                dig[len++] = t % 10;
                t /= 10;
            }
        }
        for (int i = len - 1; i >= 0; --i) {
            int d = dig[i];
            if (carry < 0) {
                carry = d;                    /* first digit of pair */
            } else {
                seq_t pair = (carry == 0) ? d
                                        : (seq_t)carry * 10 + d;
                if (--pairs_needed == 0)
                    return pair;              /* found the n-th pair */
                carry = -1;                 /* reset for next pair */
            }
        }
        m += 9;  /* move to next multiple of 9 */
    }
}
