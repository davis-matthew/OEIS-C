/*
 * A098347  A sequence derived from a Ferrers graph partition of 16.
 * https://oeis.org/A098347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 3*a(n-3) with initial values
 *            a(1)=1, a(2)=5, a(3)=16
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)16;

    /* constants in seq_t form */
    const seq_t three = (seq_t)3;

    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)5;   /* a(2) */
    seq_t a3 = (seq_t)16;  /* a(3) */
    seq_t an = a3;         /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i)
    {
        an = three * a3 + three * a2 - three * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
