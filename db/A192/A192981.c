/*
 * A192981  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4). Initial values:
 *            a(0)=1, a(1)=0, a(2)=2, a(3)=5
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;
    seq_t a0 = (seq_t)1; // a(n-4)
    seq_t a1 = (seq_t)0; // a(n-3)
    seq_t a2 = (seq_t)2; // a(n-2)
    seq_t a3 = (seq_t)5; // a(n-1)
    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = a3 * (seq_t)3 - a2 * (seq_t)2 - a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
