/*
 * A103760  Number of contiguous non-i digits in the counting numbers, for
 *          i=1.
 * https://oeis.org/A103760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = length of the nth run of non‑1 decimal digits between *
 *            successive '1' digits in the infinite concatenation of the
 *            natural * numbers starting at 0. Computed by scanning the
 *            decimal digits
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t count = 0;
    seq_t run_len = 0;
    for (idx_t num = 0; ; ++num) {
        char digits[20];
        idx_t len = 0;
        if (num == 0) {
            digits[len++] = '0';
        } else {
            idx_t tmp = num;
            while (tmp > 0) {
                digits[len++] = (char)('0' + (tmp % 10));
                tmp /= 10;
            }
        }
        for (idx_t i = len; i > 0; --i) {
            char d = digits[i - 1];
            if (d == '1') {
                if (run_len > 0) {
                    ++count;
                    if (count == n)
                        return run_len;
                    run_len = 0;
                }
            } else {
                run_len++;
            }
        }
    }
    return 0;  /* unreachable */
}
