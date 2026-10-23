/*
 * A139634  a(n) = 10*2^(n-1) - 9.
 * https://oeis.org/A139634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*2^(n-1) - 9
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(n) = 2*a(n-1) + 9 for n > 1 */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour for n < 1 is undefined */
        return 0;

    seq_t value = 1;         /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        value = value * 2 + 9;

    return value;
}
