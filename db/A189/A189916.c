/*
 * A189916  Sequence for finding the day of the week for the first day of the
 *          month in leap years.
 * https://oeis.org/A189916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = (sum_{k=1}^{n-1} days_in_month_k_leap) mod 7, where
 *            days_in_month_k_leap are the lengths of the months in a leap
 *            year (31,29,31,30,31,30,31,31,30,31,30,31)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t month_len[12] = {
        31, 29, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };
    seq_t offset = 0;
    idx_t months_before = (n - 1) % 12;   /* number of months preceding month n */
    for (idx_t i = 0; i < months_before; ++i) {
        offset = (offset + month_len[i]) % 7;
    }
    return offset;
}
