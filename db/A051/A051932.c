/*
 * A051932  Number of independent sets of nodes in graph K_7 X C_n (n > 2).
 * https://oeis.org/A051932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) + 8*a(n-2) + a(n-3), a(0) = 8, a(1) = 1, a(2) =
 *            57
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)57;

    seq_t a0 = (seq_t)8;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)57;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t an = (seq_t)6 * a2 + (seq_t)8 * a1 + a0; /* a(n) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
