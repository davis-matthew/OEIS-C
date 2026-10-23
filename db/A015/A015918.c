/*
 * A015918  Inverse of 1909th cyclotomic polynomial.
 * https://oeis.org/A015918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ⎣n/1909⎦+1 − [n≥23]·(⎣(n-23)/1909⎦+1) −
 *            [n≥83]·(⎣(n-83)/1909⎦+1) + [n≥106]·(⎣(n-106)/1909⎦+1)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic with period 1909. */

seq_t a(idx_t n)
{
    const idx_t PERIOD = 1909;

    seq_t g0 = (seq_t)(n / PERIOD) + 1;        /* g(n,0)                     */
    seq_t g23 = 0, g83 = 0, g106 = 0;

    if (n >= 23) {
        g23 = (seq_t)((n - 23) / PERIOD) + 1;  /* g(n,23)                    */
    }
    if (n >= 83) {
        g83 = (seq_t)((n - 83) / PERIOD) + 1;  /* g(n,83)                    */
    }
    if (n >= 106) {
        g106 = (seq_t)((n - 106) / PERIOD) + 1;/* g(n,106)                   */
    }

    return g0 - g23 - g83 + g106;
}
