/*
 * A083307  a(n) = (4*n^n + (n-10)^n)/5.
 * https://oeis.org/A083307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (4*n^n + (n-10)^n)/5
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1u)
            result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 4 * n^n */
    seq_t n_pow_n = pow_seq_t((seq_t)n, n);
    seq_t term1   = n_pow_n * 4;

    /* (n - 10)^n */
    seq_t term2   = pow_seq_t((seq_t)n - 10, n);

    /* Sum and divide by 5 */
    seq_t sum     = term1 + term2;
    return sum / 5;
}
