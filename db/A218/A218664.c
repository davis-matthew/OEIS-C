/*
 * A218664  Coefficients of cubic polynomials p(x+n), where p(x) = x^3 + x^2 -
 *          2*x - 1.
 * https://oeis.org/A218664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(4k) = 1 a(4k+1) = 3k + 1 a(4k+2) = 3k^2 + 2k - 2 a(4k+3) = k^3
 *            + k^2 - 2k - 1 (k = floor(n / 4)) Computation uses only integer
 *            arithmetic
 * Range:     i8 n <= 22, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)(n >> 2);        /* floor(n / 4) */
    switch ((int)(n & 3))
    {
        case 0:
            return (seq_t)1;
        case 1:
            /* 3k + 1 */
            return (seq_t)3 * k + (seq_t)1;
        case 2:
            /* 3k^2 + 2k - 2 , computed in 64‑bit to avoid overflow */
            return (seq_t)((long long)3 * k * k + (long long)2 * k - (long long)2);
        case 3:
            /* k^3 + k^2 - 2k - 1 , computed in 64‑bit to avoid overflow */
            return (seq_t)(((long long)k * k * k)
                           + ((long long)k * k)
                           - (long long)2 * k
                           - (long long)1);
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
