/*
 * A043377  Numbers having one 2 in base 6.
 * https://oeis.org/A043377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers having exactly one digit 2 in base 6
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1, so n<1 is undefined */
    seq_t cur = 1;                     /* start from 1 (0 has no digits) */
    idx_t found = 0;
    for (;;) {
        seq_t t = cur;
        idx_t cnt = 0;
        while (t > 0) {
            if ((t % 6) == 2) {
                ++cnt;
                if (cnt > 1) break;   /* more than one '2' – reject */
            }
            t /= 6;
        }
        if (cnt == 1) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;
    }
}
