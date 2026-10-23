/*
 * A026150  a(0) = a(1) = 1; a(n+2) = 2*a(n+1) + 2*a(n).
 * https://oeis.org/A026150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = 1; a(n+2) = 2*a(n+1) + 2*a(n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t an = 0;          /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        an = (seq_t)(2 * a1 + 2 * a0);
        a0 = a1;
        a1 = an;
    }

    return an;
}
