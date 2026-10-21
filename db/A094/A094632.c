/*
 * A094632  A trace sequence for a Napoleon graph.
 * https://oeis.org/A094632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=3, a(3)=4 and for n≥4 a(n) = a(n-1) +
 *            7*a(n-2) + 2*a(n-3) – 4*a(n-4)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)3;   /* a(2) */
    seq_t a3 = (seq_t)4;   /* a(3) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) = a3 + 7*a2 + 2*a1 – 4*a0  */
        seq_t temp = a3 + (seq_t)7 * a2 + (seq_t)2 * a1;
        ai      = temp - (seq_t)4 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
