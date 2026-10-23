/*
 * A045069  Numbers whose base-4 representation contains exactly one 0 and
 *          three 3's.
 * https://oeis.org/A045069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-4 representation contains exactly one 0 and three 3's (search) */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t num = 0; count < n; ++num) {
        seq_t tmp = num;
        unsigned zero = 0, three = 0;
        if (tmp == 0) {
            /* base-4 representation of 0 is "0" */
            zero = 1;
            three = 0;
        }
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % 4);
            if (digit == 0) ++zero;
            else if (digit == 3) ++three;
            tmp /= 4;
        }
        if (zero == 1 && three == 3) {
            ++count;
            if (count == n) return num;
        }
    }
    /* Should never be reached with valid n >= 1 */
    return 0;
}
