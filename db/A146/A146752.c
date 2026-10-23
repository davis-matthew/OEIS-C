/*
 * A146752  a(n) = numerator((1/2)*(1 + Product_{k=0..n-1} 2*(1 + 3*k)/(5 +
 *          6*k))).
 * https://oeis.org/A146752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((1/2)*(1 + ∏_{k=0}^{n-1} 2*(1 + 3*k)/(5 +
 *            6*k)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t numer = 1;
    seq_t denom = 1;

    for (idx_t k = 0; k < n; ++k) {
        /* compute factors for this k */
        idx_t t1 = 1 + 3 * k;           /* 1 + 3k  */
        seq_t fnum = (seq_t)(2 * t1);   /* 2*(1+3k) */

        idx_t t2 = 5 + 6 * k;           /* 5 + 6k  */
        seq_t fden = (seq_t)t2;         /* (5+6k)  */

        /* cross‑reduce to avoid overflow */
        seq_t g1 = gcd_seq(numer, fden);
        numer /= g1;
        fden   /= g1;

        seq_t g2 = gcd_seq(fnum, denom);
        fnum   /= g2;
        denom  /= g2;

        numer = numer * fnum;
        denom = denom * fden;
    }

    /* numerator of (1 + P_n)/2 = (numer + denom)/(2*denom) */
    seq_t A = numer + denom;
    seq_t B = denom * 2;

    seq_t g = gcd_seq(A, B);
    return A / g;
}
