/*
 * A189915  Sequence for finding the day of the week for the first day of the
 *          month in a common (non-leap) year.
 * https://oeis.org/A189915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = (days in January + ... + days in the previous month) mod
 *            7. The months lengths of a non‑leap year are 31, 28, 31, 30, 31,
 *            30, 31, 31, 30, 31, 30, 31. a(1)=0. For n>1,
 *            a(n)=a(n-1)+length(n-1) (mod 7)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* month lengths for a common (non‑leap) year */
    static const seq_t month_len[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    /* month index in the current year (1..12) */
    idx_t m = ((n - 1) % (idx_t)12) + 1;

    seq_t offset = 0;
    for (idx_t i = 1; i < m; ++i) {
        offset += month_len[i - 1];
        offset %= 7;
    }
    return offset;
}
