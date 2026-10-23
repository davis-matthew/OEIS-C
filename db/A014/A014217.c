/*
 * A014217  a(n) = floor(phi^n), where phi = (1+sqrt(5))/2 is the golden
 *          ratio.
 * https://oeis.org/A014217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n-1) + a(n-2) + (n & 1) for n ≥ 2
 *            (the extra +1 is added only when n is odd)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Use a rolling window: a0 = a(n-2), a1 = a(n-1). */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t an = 0;

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 + a0 + (seq_t)(i & 1);
        a0 = a1;
        a1 = an;
    }
    return an;
}
