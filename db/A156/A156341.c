/*
 * A156341  Expansion of 2*(1 - 3*x)/(1 - 12*x + 11*x^2).
 * https://oeis.org/A156341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 11*a(n-2); Initial values: a(0) = 2, a(1) =
 *            18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)18;

    seq_t a0 = (seq_t)2;   /* a(n-2) */
    seq_t a1 = (seq_t)18;  /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t t1 = a1 * (seq_t)12;
        seq_t t2 = a0 * (seq_t)11;
        a0 = a1;
        a1 = t1 - t2;     /* a(n) = 12*a1 - 11*a0 */
    }
    return a1;
}
