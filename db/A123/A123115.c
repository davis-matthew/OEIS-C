/*
 * A123115  Values of Y satisfying the equation (X-Y)^4 - 2*X*Y = 0 with
 *          integer X >= Y >= 0.
 * https://oeis.org/A123115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*a(n-1) - 206*a(n-2) + 40*a(n-3) - a(n-4) (n ≥ 4)
 *            Initial values: a(0)=0, a(1)=2, a(2)=96, a(3)=3430
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)96;
    if (n == 3) return (seq_t)3430;

    seq_t a_minus4 = (seq_t)0;
    seq_t a_minus3 = (seq_t)2;
    seq_t a_minus2 = (seq_t)96;
    seq_t a_minus1 = (seq_t)3430;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t curr = (seq_t)40 * a_minus1
                   - (seq_t)206 * a_minus2
                   + (seq_t)40 * a_minus3
                   - a_minus4;
        a_minus4 = a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = curr;
    }
    return a_minus1;
}
