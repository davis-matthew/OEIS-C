/*
 * A081142  12th binomial transform of (0,0,1,0,0,0,...).
 * https://oeis.org/A081142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12^(n-2) * n*(n-1)/2 (a(0)=a(1)=0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while(exp)
    {
        if(exp & 1U) result *= base;
        if(exp > 1U) base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if(n < 2) return 0;
    seq_t binom = (seq_t)n * ((seq_t)n - 1) / 2;
    seq_t pow12 = pow_seq_t(12, n - 2);
    return binom * pow12;
}
