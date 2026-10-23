/*
 * A171800  a(n) = ((n+1)*2^n + 1)*(2^n + 1)^(n-1).
 * https://oeis.org/A171800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)*2^n + 1)*(2^n + 1)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t pow2n = pow_seq((seq_t)2, n);
    seq_t t1 = ((seq_t)(n + 1) * pow2n) + (seq_t)1;
    seq_t t2 = pow_seq(pow2n + (seq_t)1, n - 1);
    return t1 * t2;
}
