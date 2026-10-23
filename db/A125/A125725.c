/*
 * A125725  Numbers whose base-7 representation is 222....2.
 * https://oeis.org/A125725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (7^(n-1) - 1)/3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0,  a(n) = 7*a(n-1) + 2   */

seq_t a(idx_t n)
{
    /* Handle the first term directly */
    if (n == 1)
        return 0;

    seq_t val = 0;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        val = 7 * val + 2;  /* a(i) = 7*a(i−1) + 2 */

    return val;
}
