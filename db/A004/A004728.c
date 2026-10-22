/*
 * A004728  Delete all 9's from the sequence of nonnegative integers.
 * https://oeis.org/A004728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = m with all its 9 digits deleted, where m is the n-th
 *            nonnegative integer (from 0) that is not all 9's (9, 99, 999,
 *            ... become empty and are skipped)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n;
    idx_t r = 9;
    /* skip numbers made only of 9's: they vanish entirely */
    while (m >= r) {
        m++;
        if (r > ((idx_t)-1 - 9) / 10) break;
        r = r * 10 + 9;
    }

    seq_t result = 0;
    seq_t power = 1;
    while (m > 0) {
        idx_t d = m % 10;
        m /= 10;
        if (d != 9) {
            result += (seq_t)d * power;
            power *= 10;
        }
    }
    return result;
}
