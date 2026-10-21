/*
 * A212898  Number of (w,x,y,z) with all terms in {0,...,n} and (least
 *          gapsize)<=2.
 * https://oeis.org/A212898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n^3 - 48*n^2 + 144*n - 161 for n>4, with initial
 *            values a(0)=1, a(1)=16, a(2)=81, a(3)=254, a(4)=609
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 660, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial segment directly. */
    if (n <= 4) {
        static const seq_t table[5] = {
            (seq_t)1,   /* a(0) */
            (seq_t)16,  /* a(1) */
            (seq_t)81,  /* a(2) */
            (seq_t)254, /* a(3) */
            (seq_t)609  /* a(4) */
        };
        return table[n];
    }

    /* For n > 4 use the cubic polynomial. */
    seq_t x  = (seq_t)n;
    seq_t n2 = x * x;       /* n^2 */
    seq_t n3 = n2 * x;      /* n^3 */

    seq_t res = 15 * n3;    /* 15*n^3 */
    res -= 48 * n2;         /* -48*n^2 */
    res += 144 * x;         /* +144*n */
    res -= 161;             /* -161 */

    return res;
}
