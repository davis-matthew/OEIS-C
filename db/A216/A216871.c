/*
 * A216871  16k^2-16k-4 interleaved with 16k^2+4 for k>=0.
 * https://oeis.org/A216871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(2*n*(n-2)-3*(-1)^n+1)
 * Range:     i8 n <= 6, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t to avoid intermediate overflow */
    seq_t sn = (seq_t)n;
    /* compute (-1)^n */
    seq_t neg_one_pow = ((n & 1) ? (seq_t)(-1) : (seq_t)1);

    /* compute 2*n*(n-2) */
    seq_t term1 = 2 * sn * (sn - 2);

    /* compute -3*(-1)^n */
    seq_t term2 = -3 * neg_one_pow;

    /* combine parts */
    seq_t result = 2 * (term1 + term2 + 1);

    return result;
}
