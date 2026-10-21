/*
 * A094647  a(n) = n^(2n) - (2n)^n.
 * https://oeis.org/A094647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^(2n) - (2n)^n
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur = base;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result = result * cur;
        exp >>= (idx_t)1;
        if (exp)
            cur = cur * cur;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The sequence is only defined for n >= 1.  For n == 0 we return 0
       to satisfy the function signature, though it is not used. */
    if (n == 0)
        return (seq_t)0;

    /* Compute n^n once and square it to obtain n^(2n). */
    seq_t pow_n_n = pow_seq_t((seq_t)n, n);
    seq_t pow_n_2n = pow_n_n * pow_n_n;

    /* Compute (2n)^n. */
    seq_t pow_2n_n = pow_seq_t((seq_t)(n * (idx_t)2), n);

    return pow_n_2n - pow_2n_n;
}
