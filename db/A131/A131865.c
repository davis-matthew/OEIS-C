/*
 * A131865  Partial sums of powers of 16.
 * https://oeis.org/A131865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16^(n+1) - 1) / 15 Recurrence: a(0) = 1, a(n) = 16 *
 *            a(n-1) + 1 for n > 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                 /* a(0) */
    for (idx_t i = 0; i < n; ++i)  /* compute a(1) … a(n) */
        res = res * 16 + 1;
    return res;
}
