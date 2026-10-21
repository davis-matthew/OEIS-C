/*
 * A216875  20k^2-40k+10 interleaved with 20k^2-20k+10 for k>=0.
 * https://oeis.org/A216875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(2k) = 20*k^2 - 40*k + 10 Formula: a(2k+1) = 20*k^2 - 20*k + 10
 *            (k >= 0) Equivalent to a(n) = (5/2)*(2*n*(n-4)-3*(-1)^n+7) for
 *            all n >= 0
 * Range:     i8 n <= 6, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k      = n >> 1;          /* k = floor(n/2)   */
    seq_t kk     = (seq_t)k;        /* cast index to seq_t */
    seq_t term   = (seq_t)20 * kk * kk;   /* 20*k^2 */

    if ((n & 1) == 0) {            /* even n -> a(2k)   */
        return term - (seq_t)40 * kk + (seq_t)10;
    } else {                       /* odd n  -> a(2k+1) */
        return term - (seq_t)20 * kk + (seq_t)10;
    }
}
