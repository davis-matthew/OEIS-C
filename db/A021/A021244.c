/*
 * A021244  Expansion of 1/((1-x)*(1-2*x)*(1-7*x)*(1-11*x)).
 * https://oeis.org/A021244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 133*a(n-2) + 267*a(n-3) - 154*a(n-4) Initial
 *            values: a(0)=1, a(1)=21, a(2)=308, a(3)=3942
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)21;
    if (n == 2u) return (seq_t)308;
    if (n == 3u) return (seq_t)3942;

    /* Variables to hold the last four computed terms
       a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)21;
    seq_t a2 = (seq_t)308;
    seq_t a3 = (seq_t)3942;
    seq_t ai;

    idx_t i;
    for (i = 4u; i <= n; ++i) {
        ai = 21 * a3 - 133 * a2 + 267 * a1 - 154 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}
