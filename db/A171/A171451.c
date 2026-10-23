/*
 * A171451  a(n) = 2^C(n, 2) * 2^floor(n/3).
 * https://oeis.org/A171451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^C(n, 2) * 2^floor(n/3)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_exp(seq_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (e > 0)
    {
        if (e & (seq_t)1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t binom;
    if (n < 2)
        binom = (seq_t)0;
    else
        binom = ((seq_t)n * ((seq_t)n - (seq_t)1)) / (seq_t)2;
    seq_t exp = binom + (seq_t)(n / 3);
    return pow2_exp(exp);
}
