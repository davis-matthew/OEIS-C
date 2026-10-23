/*
 * A127147  Q(n,5), where Q(m,k) is defined in A127080 and A127137.
 * https://oeis.org/A127147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 - 16*n + 43
 * Range:     i8 n <= 20, i16 n <= 189, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;                /* cast index to seq_t */
    return n_t * n_t - (seq_t)16 * n_t + (seq_t)43;
}
