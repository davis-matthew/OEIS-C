/*
 * A122475  Time every 15 minutes from 1:55.
 * https://oeis.org/A122475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = hour*100 + minute where the time starts at 1:55 and each
 *            step adds 15 minutes. The hour is taken modulo 12, with 0
 *            replaced by 12. Equivalent: let t = 115 + 15*(n-1) (minutes
 *            since 0:00), hour = ((t/60) % 12 == 0) ? 12 : (t/60) % 12,
 *            minute = t % 60
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* sequence offset is 1; safeguard against 0 */
        return 0;

    /* total minutes from midnight at the n-th term */
    seq_t total = (seq_t)115 + (seq_t)15 * (seq_t)(n - 1);

    seq_t hour_raw = total / 60;
    seq_t hour_mod = hour_raw % (seq_t)12;
    seq_t hour = hour_mod == 0 ? 12 : hour_mod;

    seq_t minute = total % (seq_t)60;

    return hour * (seq_t)100 + minute;
}
