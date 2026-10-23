/*
 * A115006  Row 2 of array in A114999.
 * https://oeis.org/A115006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)^2/4) + n*(n+1)
 * Range:     u8 n <= 13, u16 n <= 228, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = nn * (nn + 1);                    /* n*(n+1) */
    seq_t term2 = ((nn + 1) * (nn + 1)) / 4;        /* floor((n+1)^2/4) */
    return term1 + term2;
}
