/*
 * A179466  Coefficients of
 *          x+(x+1)^2+(x+2)^3+(x+3)^4+(x+4)^5+(x+5)^6+(x+6)^7+(x+7)^8, highest
 *          power first.
 * https://oeis.org/A179466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini, easy
 * Formula:   a(n) = coefficient of x^(9-n) in x + (x+1)^2 + (x+2)^3 + (x+3)^4
 *            + (x+4)^5 + (x+5)^6 + (x+6)^7 + (x+7)^8 (highest power first).
 *            Therefore a(1)=coeff of x^8, a(9)=coeff of x^0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t coeff[9];
    static int initialized = 0;

    if (!initialized) {
        /* compute coefficients once */
        for (int k = 0; k <= 7; ++k) {
            int d = k + 1;          /* degree of (x+k)^d */
            for (int e = 0; e <= d; ++e) {   /* e : power of x in the term */
                /* binomial coefficient C(d,e) */
                seq_t c = 1;
                int r = e;
                if (r > d - r) r = d - r;          /* symmetry */
                for (int i = 1; i <= r; ++i) {
                    c = (c * (seq_t)(d + 1 - i)) / (seq_t)i;
                }

                /* power of k: k^(d-e) */
                seq_t p = 1;
                for (int t = 0; t < d - e; ++t) {
                    p *= (seq_t)k;
                }

                coeff[e] += c * p;
            }
        }
        initialized = 1;
    }

    if (n < 1 || n > 9) {
        return (seq_t)0;
    }
    int e = 9 - (int)n;     /* coefficient of x^e */
    return coeff[e];
}
