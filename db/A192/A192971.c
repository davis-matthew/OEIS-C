/*
 * A192971  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4), with initial
 *            values a(0)=1, a(1)=2, a(2)=9, a(3)=21
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[4] = { (seq_t)1, (seq_t)2, (seq_t)9, (seq_t)21 };
    if (n < 4) return init[n];

    seq_t a0 = init[0], a1 = init[1], a2 = init[2], a3 = init[3];
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)3 * a3 - (seq_t)2 * a2 - a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
