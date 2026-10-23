/*
 * A037783  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,2,0.
 * https://oeis.org/A037783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the integer whose base‑8 digits are the first n terms of
 *            the periodic sequence 3,1,2,0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* pattern repeats every 4 digits: 3,1,2,0 */
    static const int pat[4] = {3, 1, 2, 0};

    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        /* multiply current value by 8 (shift left one base‑8 digit)
           then add the next digit from the pattern */
        res = res * 8 + pat[i & 3];
    }
    return res;
}
