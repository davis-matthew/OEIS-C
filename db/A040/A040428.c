/*
 * A040428  Continued fraction for sqrt(450).
 * https://oeis.org/A040428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continuous fraction expansion terms for √450.
   a(0) = 21;
   for n > 0: a(n) = period[(n-1) mod 8] where
   period = {4, 1, 2, 4, 2, 1, 4, 42}.  */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)21;
    /* The period length is 8, so indexing can use a bit mask. */
    idx_t idx = (n - 1) & 7;
    static const seq_t period[8] = {4, 1, 2, 4, 2, 1, 4, 42};
    return period[idx];
}
