/*
 * A073304  Remaining days in non-leap year at end of n-th month.
 * https://oeis.org/A073304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 365 - ∑_{k=0}^{n-1} month_days[k] * where month_days =
 *            [31,28,31,30,31,30,31,31,30,31,30,31]. * For n > 12 the sequence
 *            stays at 0
 * Range:     u8 none, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the number of days in each month of a non‑leap year */
    static const idx_t month_days[12] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    /* n larger than the year means no days remaining */
    if (n >= 12)
        return (seq_t)0;

    seq_t remaining = (seq_t)365;
    for (idx_t i = 0; i < n; ++i)
        remaining -= (seq_t)month_days[i];

    return remaining;
}
