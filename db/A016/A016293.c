/*
 * A016293  Expansion of 1 / ((1-2*x) * (1-4*x) * (1-11*x)).
 * https://oeis.org/A016293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 74*a(n-2) + 88*a(n-3); Initial values:
 *            a(0)=1, a(1)=17, a(2)=215
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)17;
    if (n == 2)
        return (seq_t)215;

    seq_t a0 = 1;
    seq_t a1 = 17;
    seq_t a2 = 215;
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = ((seq_t)17) * a2 - ((seq_t)74) * a1 + ((seq_t)88) * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
