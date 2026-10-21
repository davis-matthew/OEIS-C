/*
 * A083575  a(0) = 6; for n>0, a(n) = 2*a(n-1) - 1.
 * https://oeis.org/A083575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n + 1 Recurrence: a(0) = 6; for n > 0, a(n) =
 *            2*a(n-1) – 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 6;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * 2 - 1;  /* recurrence step */
    }
    return val;
}
