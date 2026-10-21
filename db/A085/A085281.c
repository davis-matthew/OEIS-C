/*
 * A085281  Expansion of (1 - 3*x + x^2)/((1-2*x)*(1-3*x)).
 * https://oeis.org/A085281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(n-1) + 2^(n-1) for n > 0; a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= (idx_t)1;
        if (exp)               /* avoid unnecessary multiply at the end */
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    idx_t e = n - (idx_t)1;
    seq_t p3 = pow_seq_t((seq_t)3, e);
    seq_t p2 = pow_seq_t((seq_t)2, e);
    return p3 + p2;
}
