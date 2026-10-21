/*
 * A073305  Remaining days in leap year at end of n-th month.
 * https://oeis.org/A073305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 366 – sum_{i=1}^{n} monthDays[i] monthDays =
 *            {31,29,31,30,31,30,31,31,30,31,30,31} If n>12, a(n) = 0
 * Range:     u8 none, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Days in each month of a leap year, indexed from 1. */
    static const seq_t monthDays[13] = {
        0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    /* For months beyond December, no days remain. */
    if (n > 12)
        return (seq_t)0;

    /* Accumulate days up to month n. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += monthDays[i];

    return (seq_t)366 - sum;
}
