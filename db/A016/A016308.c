/*
 * A016308  Expansion of 1/((1-2*x)*(1-6*x)*(1-11*x)).
 * https://oeis.org/A016308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*2^n - 81*6^n + 121*11^n)/45 Recurrence: a(0)=1,
 *            a(1)=19, a(2)=261, a(n)=19*a(n-1) - 100*a(n-2) + 132*a(n-3) for
 *            n>2 (all operations are performed on seq_t, which is either a
 *            built‑in unsigned integer type or an arbitrary‑precision integer
 *            class)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)19;
    if (n == (idx_t)2) return (seq_t)261;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)19;
    seq_t a2 = (seq_t)261;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t t = (seq_t)19 * a2 + (seq_t)132 * a0 - (seq_t)100 * a1;
        a0 = a1;
        a1 = a2;
        a2 = t;
    }
    return a2;
}
