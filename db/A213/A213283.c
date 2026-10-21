/*
 * A213283  Number of 4-length words w over n-ary alphabet such that for every
 *          prefix z of w we have #(z,a_i) = 0 or #(z,a_i) >= #(z,a_j) for all
 *          j>i and #(z,a_i) counts the occurrences of the i-th letter in z.
 * https://oeis.org/A213283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-9 + 17*n - 8*n^2 + 2*n^3) / 2
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 220, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n * (seq_t)n;            /* n^2   */
    seq_t n3  = t * (seq_t)n;                   /* n^3   */
    seq_t expr = (seq_t)2 * n3
                 - (seq_t)8 * t
                 + (seq_t)17 * (seq_t)n
                 - (seq_t)9;                   /* 2n^3-8n^2+17n-9 */
    seq_t numer = (seq_t)n * expr;              /* n*(...) */
    return numer / 2;                           /* exact division */
}
