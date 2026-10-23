/*
 * A199679  a(n) = (11*9^n+1)/4.
 * https://oeis.org/A199679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*9^n + 1)/4 Recurrence used: a(0) = 3, a(n) = 9*a(n-1)
 *            - 2 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 3;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) /* compute up to a(n) */
        val = val * 9u - 2u;       /* recurrence step */
    return val;
}
