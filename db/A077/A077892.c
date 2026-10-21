/*
 * A077892  Expansion of (1-x)^(-1)/(1+2*x^2-x^3).
 * https://oeis.org/A077892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n‑1) - 2*a(n‑2) + 3*a(n‑3) - a(n‑4) a(0)=1, a(1)=1,
 *            a(2)=-1, a(3)=0
 * Range:     i8 n <= 14, i16 n <= 27, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)0;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)-1;
    seq_t a3 = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = a3 - (seq_t)2 * a2 + (seq_t)3 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
