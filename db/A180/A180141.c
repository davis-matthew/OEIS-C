/*
 * A180141  Eight rooks and one berserker on a 3 X 3 chessboard. G.f.: (1 + x
 *          - 2*x^2)/(1 - 3*x - 6*x^2).
 * https://oeis.org/A180141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=16; for n>=3: a(n) = 3*a(n-1) + 6*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;

    seq_t a0 = (seq_t)1;   /* a(n-3) when n=3 */
    seq_t a1 = (seq_t)4;   /* a(n-2) when n=3 */
    seq_t a2 = (seq_t)16;  /* a(n-1) when n=3 */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t an = (seq_t)3 * a2 + (seq_t)6 * a1;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
