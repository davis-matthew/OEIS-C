/*
 * A041449  Denominators of continued fraction convergents to sqrt(240).
 * https://oeis.org/A041449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=61, a(3)=124, a(n)=62*a(n-2)-a(n-4) for n>3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)61;
    if (n == 3) return (seq_t)124;

    /* t0..t3 will hold four consecutive terms a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t t0 = (seq_t)1;   /* a(0) */
    seq_t t1 = (seq_t)2;   /* a(1) */
    seq_t t2 = (seq_t)61;  /* a(2) */
    seq_t t3 = (seq_t)124; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)62 * t2 - t0;  /* a(i) = 62*a(i-2) - a(i-4) */
        t0 = t1;   /* shift window forward */
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
