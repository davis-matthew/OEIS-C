/*
 * A179465  Integers of alternate form
 *          x+(x+1)^2+(x+2)^3+(x+3)^4+(x+4)^5+(x+5)^6.
 * https://oeis.org/A179465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^(7‑n) in the polynomial * x + (x+1)^2 +
 *            (x+2)^3 + (x+3)^4 + (x+4)^5 + (x+5)^6 * The polynomial expands
 *            to * x^6 + 31*x^5 + 396*x^4 + 2673*x^3 + 10076*x^2 + 20153*x +
 *            16739 * so a(1)=1, a(2)=31, … , a(7)=16739
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients stored as [x^0 … x^6] */
    static seq_t coeff[7];
    static int initialized = 0;

    if (!initialized) {
        /* zero array */
        for (int i = 0; i < 7; ++i)
            coeff[i] = 0;

        /* add the linear term x */
        coeff[1] = 1;

        /* binomial coefficients for m≤6 */
        static const int binom[7][7] = {
            {1,0,0,0,0,0,0},
            {1,1,0,0,0,0,0},
            {1,2,1,0,0,0,0},
            {1,3,3,1,0,0,0},
            {1,4,6,4,1,0,0},
            {1,5,10,10,5,1,0},
            {1,6,15,20,15,6,1}
        };

        /* add each term (x + a)^m for m=2..6, a=m-1 */
        for (int m = 2; m <= 6; ++m) {
            seq_t a = (seq_t)(m - 1);
            for (int j = 0; j <= m; ++j) {
                /* compute a^(m-j) */
                seq_t pow = 1;
                int exp = m - j;
                for (int e = 0; e < exp; ++e)
                    pow *= a;

                coeff[j] += (seq_t)binom[m][j] * pow;
            }
        }
        initialized = 1;
    }

    if (n >= 1 && n <= 7)
        return coeff[7 - n];   /* coeff[6] is x^6, coeff[0] is constant term */

    /* Outside the known range, return zero (not part of the sequence) */
    return 0;
}
