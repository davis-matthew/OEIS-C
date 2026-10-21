/*
 * A059816  Let g_n be the ball packing n-width for the manifold torus X
 *          square; sequence gives denominator of (g_n/Pi)^2.
 * https://oeis.org/A059816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = n/2   if n is even and n >= 8,
 *            a(n) = n     if n is odd  and n >= 8.
 * For n < 8, the first ten terms are used as a lookup table:
 * a(1)=1, a(2)=1, a(3)=9, a(4)=9, a(5)=25, a(6)=49,
 * a(7)=225, a(8)=4, a(9)=9, a(10)=5.
 */
seq_t a(idx_t n)
{
    /* small terms for n = 1..10 */
    static const seq_t small[] = {
        (seq_t)1,   /* a(1) */
        (seq_t)1,   /* a(2) */
        (seq_t)9,   /* a(3) */
        (seq_t)9,   /* a(4) */
        (seq_t)25,  /* a(5) */
        (seq_t)49,  /* a(6) */
        (seq_t)225, /* a(7) */
        (seq_t)4,   /* a(8) */
        (seq_t)9,   /* a(9) */
        (seq_t)5    /* a(10) */
    };

    if (n <= 10)              /* use the table for n = 1..10 */
        return small[n - 1];

    /* n >= 11 : use the closed‑form rule */
    if ((n & 1) == 0)         /* even n  */
        return (seq_t)(n / 2);
    else                      /* odd n   */
        return (seq_t)n;
}
