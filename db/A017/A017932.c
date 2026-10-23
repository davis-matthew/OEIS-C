/*
 * A017932  Expansion of 1/((1-3x)*(1-6x)*(1-8x)).
 * https://oeis.org/A017932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 90*a(n-2) + 144*a(n-3) with a(0)=1, a(1)=17,
 *            a(2)=199
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)199;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)17;  /* a(n-2) */
    seq_t a2 = (seq_t)199; /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)17 * a2 - (seq_t)90 * a1 + (seq_t)144 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
