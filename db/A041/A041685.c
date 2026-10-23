/*
 * A041685  Denominators of continued fraction convergents to sqrt(362).
 * https://oeis.org/A041685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 38*a(n‑1) + a(n‑2), a(0)=1, a(1)=38
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)38;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)38;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a2 = a1 * (seq_t)38 + a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
