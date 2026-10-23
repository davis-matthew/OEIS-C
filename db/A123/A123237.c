/*
 * A123237  A000316-like neo-Hankel matrix determinant sequence.
 * https://oeis.org/A123237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) = (-1)^{n(n-1)/2} * ∏_{i=1}^{n} floor( (n+1 + 2*i*(n-i+1))
 *            / 4 )
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 8, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A123237. */
seq_t a(idx_t n)
{
    /* Determine sign: negative when C(n,2) is odd, i.e. when n%4==2 or 3. */
    int sign = ((n % 4 == 2) || (n % 4 == 3)) ? -1 : 1;

    seq_t result = (seq_t)1;
    seq_t n_se   = (seq_t)n;          /* cast n to seq_t for arithmetic */
    seq_t n_plus = n_se + (seq_t)1;   /* n + 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t i_se      = (seq_t)i;
        seq_t term_num  = n_plus + (seq_t)2 * i_se * (n_se - i_se + (seq_t)1);
        seq_t term      = term_num / (seq_t)4;          /* floor division */
        result *= term;
    }

    if (sign < 0)
        result = -result;
    return result;
}
