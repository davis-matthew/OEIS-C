/*
 * A188021  Expansion of (x^2)/[(1-x)*(1-3*x^2-x^3)].
 * https://oeis.org/A188021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+3*a(n-2)-2*a(n-3)-a(n-4); Initial values: a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=1
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 72, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[4] = { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)1 };
    if (n < 4) {
        return init[n];
    }

    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t res;

    for (idx_t i = 4; i <= n; ++i) {
        res = a3 + a2 * (seq_t)3 - a1 * (seq_t)2 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = res;
    }
    return a3;
}
