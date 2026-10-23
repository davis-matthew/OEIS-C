/*
 * A100477  a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3) if n>=3, otherwise a(n) = n.
 * https://oeis.org/A100477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for n<3 a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3) for n>=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;
    if (n == 1U) return (seq_t)1;
    if (n == 2U) return (seq_t)2;

    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t an = a2;

    for (idx_t i = 3U; i <= n; ++i)
    {
        an = 3 * a2 + 2 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
