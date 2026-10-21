/*
 * A081280  Binomial transform of Chebyshev coefficients A006974.
 * https://oeis.org/A081280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 + 24*n^3 + 164*n^2 + 378*n + 243) * 3^(n-5)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work in the integer type seq_t to satisfy the compiler for both
       fixed‑width and arbitrary‑precision builds. */
    seq_t nn   = (seq_t)n;
    seq_t poly = nn*nn*nn*nn               /* n^4          */
               + (seq_t)24 * nn*nn*nn     /* 24n^3       */
               + (seq_t)164 * nn*nn       /* 164n^2      */
               + (seq_t)378 * nn          /* 378n        */
               + 243;                    /* constant    */

    /* Compute the power of 3.  For n < 5 the exponent is negative, so
       we divide by 3^(5-n).  The result is always an integer. */
    if (n >= 5) {
        idx_t e = n - 5;
        for (idx_t i = 0; i < e; ++i)
            poly *= 3;
    } else {
        idx_t e = 5 - n;
        for (idx_t i = 0; i < e; ++i)
            poly /= 3;
    }

    return poly;
}
