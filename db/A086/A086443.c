/*
 * A086443  Expansion of x^2/((1-4*x)*(1-3*x)^2).
 * https://oeis.org/A086443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n - 3^n - n*3^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0)
    {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* a(0) = 0 */

    seq_t p4        = pow_seq_t((seq_t)4, n);
    seq_t p3        = pow_seq_t((seq_t)3, n);
    seq_t p3prev    = pow_seq_t((seq_t)3, n - 1);

    seq_t term = p4 - p3 - ((seq_t)n * p3prev);
    return term;
}
