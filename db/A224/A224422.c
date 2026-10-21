/*
 * A224422  Expansion of (1-x)*(1-3*x)/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A224422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=20, a(3)=75, a(4)=275; For n≥5: a(n) =
 *            9*a(n-1) – 28*a(n-2) + 35*a(n-3) – 15*a(n-4) + a(n-5)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)75;
    if (n == 4) return (seq_t)275;

    seq_t a0 = 1, a1 = 5, a2 = 20, a3 = 75, a4 = 275;
    for (idx_t i = 5; i <= n; i++) {
        seq_t next = 9 * a4 - 28 * a3 + 35 * a2 - 15 * a1 + a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = next;
    }
    return a4;
}
