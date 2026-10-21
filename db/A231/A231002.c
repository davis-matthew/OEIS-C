/*
 * A231002  Number of years after which it is possible to have a date falling
 *          on same day of the week, but the entire year does not have the
 *          same calendar, in the Julian calendar.
 * https://oeis.org/A231002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -7 + 2*(-1)^n + 14*n
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute a(n) = 14*n - ( (n odd)?9:5 ) */
    seq_t res = ((seq_t)n) * (seq_t)14;
    res -= (n & 1) ? (seq_t)9 : (seq_t)5;
    return res;
}
