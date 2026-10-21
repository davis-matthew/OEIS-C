/*
 * A098879  a(n) = (2^n - 1)^5 - 2.
 * https://oeis.org/A098879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (2^n - 1)^5 - 2 (OEIS A098879)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t p = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        p *= (seq_t)2;
    }

    /* (2^n - 1) */
    p -= (seq_t)1;

    /* (2^n - 1)^5  -> multiply p by itself 5 times */
    seq_t res = p * p;          /* p^2 */
    res = res * p;              /* p^3 */
    res = res * p;              /* p^4 */
    res = res * p;              /* p^5 */

    /* - 2 */
    res -= (seq_t)2;
    return res;
}
