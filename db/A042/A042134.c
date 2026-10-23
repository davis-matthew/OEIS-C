/*
 * A042134  Numerators of continued fraction convergents to sqrt(592).
 * https://oeis.org/A042134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 146*a(n-2) - a(n-4); Initial values: a(0) = 24, a(1) =
 *            73, a(2) = 3528, a(3) = 10657
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small index cases */
    if (n == 0) return (seq_t)24;
    if (n == 1) return (seq_t)73;
    if (n == 2) return (seq_t)3528;
    if (n == 3) return (seq_t)10657;

    /* sliding window: t0=a(i-4), t1=a(i-3), t2=a(i-2), t3=a(i-1) */
    seq_t t0 = (seq_t)24;      /* a(0) */
    seq_t t1 = (seq_t)73;      /* a(1) */
    seq_t t2 = (seq_t)3528;    /* a(2) */
    seq_t t3 = (seq_t)10657;   /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = 146 * t2 - t0;   /* a(i) = 146*a(i-2) - a(i-4) */
        /* shift window */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = new_val;
    }

    return t3; /* for i==n, t3 holds a(n) */
}
