/*
 * A026308  Expansion of 1/((1-2*x)*(1-5*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A026308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=30, a(2)=597, a(3)=9974; for n ≥ 4: a(n) =
 *            30*a(n-1) - 303*a(n-2) + 1154*a(n-3) - 1320*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a0 = 1;
    seq_t a1 = 30;
    seq_t a2 = 597;
    seq_t a3 = 9974;
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    seq_t cur;
    for (idx_t i = 4; i <= n; ++i) {
        cur = 30 * a3 - 303 * a2 + 1154 * a1 - 1320 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3;
}
