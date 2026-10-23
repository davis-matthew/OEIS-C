/*
 * A017897  Expansion of 1/((1-3*x)*(1-5*x)*(1-9*x)).
 * https://oeis.org/A017897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 87*a(n-2) + 135*a(n-3); a(0)=1, a(1)=17,
 *            a(2)=202
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)202;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)17;  /* a(n-2) */
    seq_t a2 = (seq_t)202; /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)17 * a2 - (seq_t)87 * a1 + (seq_t)135 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
