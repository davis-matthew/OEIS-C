/*
 * A186810  Partial sums of A009940.
 * https://oeis.org/A186810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) = Sum_{k=0..n} k!*LaguerreL(k,1). Recurrence: a(n) = a(n-1)
 *            + t_n, where t_0 = 1, t_1 = 0, and for k >= 1, t_{k+1} = 2*k*t_k
 *            - k*k*t_{k-1}
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t t_prevprev = (seq_t)1; /* t_0   */
    seq_t t_prev     = (seq_t)0; /* t_1   */
    seq_t sum        = t_prevprev + t_prev; /* a(1) */

    for (idx_t k = 1; k < n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t term  = 2 * k_seq * t_prev - k_seq * k_seq * t_prevprev;
        sum += term;
        t_prevprev = t_prev;
        t_prev = term;
    }
    return sum;
}
