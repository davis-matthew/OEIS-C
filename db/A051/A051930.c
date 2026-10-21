/*
 * A051930  Number of independent sets of vertices in graph K_5 X C_n (n > 2).
 * https://oeis.org/A051930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=6, a(1)=1, a(2)=31, a(n)=4*a(n-1)+6*a(n-2)+a(n-3) for n≥3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)31;

    seq_t a0 = (seq_t)6;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)31;  /* a(n-1) */
    seq_t an = a2;         /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)4 * a2 + (seq_t)6 * a1 + a0; /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
