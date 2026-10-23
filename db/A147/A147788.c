/*
 * A147788  a(n) = floor(2*(3/2)^n).
 * https://oeis.org/A147788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*(3/2)^n) Equivalent: a(0)=2, for n>0 a(n)= 3^n /
 *            2^(n‑1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            res = res * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)2;

    seq_t numerator   = pow_seq_t((seq_t)3, n);
    seq_t denominator = pow_seq_t((seq_t)2, n - (idx_t)1);

    return numerator / denominator;
}
