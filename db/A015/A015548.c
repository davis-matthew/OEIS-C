/*
 * A015548  Expansion of x/(1 - 5*x - 12*x^2).
 * https://oeis.org/A015548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1, a(n) = 5*a(n-1) + 12*a(n-2) for n ≥ 2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)0;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = (seq_t)5 * a1 + (seq_t)12 * a0;
        a0 = a1;
        a1 = temp;
    }
    return a1;
}
