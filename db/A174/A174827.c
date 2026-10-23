/*
 * A174827  Hankel transform of A098278.
 * https://oeis.org/A174827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product{k=0..n,
 *            ((k+1)*(2*k+1)*(2*k+2)*floor((2*k+3)/2))^(n-k)}
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast exponentiation for seq_t base with idx_t exponent */
static seq_t pow_seq_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        if (exp > 1)
            base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t k1 = k + (idx_t)1;               /* k+1 as idx_t */
        seq_t kk1 = (seq_t)k1;                /* (k+1) as seq_t */
        /* Base = (k+1)^2 * (2k+1) * (2k+2)  (since floor((2k+3)/2)=k+1) */
        seq_t base = kk1 * kk1 * (seq_t)(2*k + (idx_t)1) * (seq_t)(2*k + (idx_t)2);
        idx_t exp = n - k;
        if (exp > (idx_t)0)
            result *= pow_seq_seq(base, exp);
    }
    return result;
}
