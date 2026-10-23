/*
 * A023424  Expansion of (1+2*x+3*x^2+4*x^3+5*x^4)/(1-x-x^2-x^3-x^4-x^5).
 * https://oeis.org/A023424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5), a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=15, a(4)=31
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 64, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first five terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)31;

    /* Use a sliding window of the last five values. */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)7;
    seq_t a3 = (seq_t)15;
    seq_t a4 = (seq_t)31;
    seq_t a5;

    for (idx_t i = 5; i <= n; ++i) {
        a5 = a0 + a1 + a2 + a3 + a4;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }
    return a4;
}
