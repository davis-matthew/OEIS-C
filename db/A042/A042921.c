/*
 * A042921  Denominators of continued fraction convergents to sqrt(992).
 * https://oeis.org/A042921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 126*a(n-2) - a(n-4) for n>3 Initial values: a(0)=1,
 *            a(1)=2, a(2)=125, a(3)=252
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)125;
    if (n == 3u) return (seq_t)252;

    /* a(n) depends only on a(n-4), a(n-2) */
    seq_t a0 = (seq_t)1;    /* a(n-4) for current n */
    seq_t a1 = (seq_t)2;    /* a(n-3) unused but kept for shifting */
    seq_t a2 = (seq_t)125;  /* a(n-2) for current n */
    seq_t a3 = (seq_t)252;  /* a(n-1) unused but kept for shifting */
    seq_t an = (seq_t)0;

    for (idx_t i = 4u; i <= n; ++i) {
        an = (seq_t)126 * a2 - a0;  /* recurrence */
        /* shift values for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
