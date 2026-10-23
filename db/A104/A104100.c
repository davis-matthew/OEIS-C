/*
 * A104100  Expansion of g.f. x*(-20-37*x+3*x^2)/(-1+x+19*x^2+10*x^3+x^4).
 * https://oeis.org/A104100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 19*a(n-2) + 10*a(n-3) + a(n-4) Initial values:
 *            a(0)=0, a(1)=20, a(2)=57, a(3)=434
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)57;
    if (n == 3) return (seq_t)434;

    seq_t a0 = (seq_t)0;   // a(n-4)
    seq_t a1 = (seq_t)20;  // a(n-3)
    seq_t a2 = (seq_t)57;  // a(n-2)
    seq_t a3 = (seq_t)434; // a(n-1)
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = a3 + (seq_t)19 * a2 + (seq_t)10 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
