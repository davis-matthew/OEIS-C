/*
 * A184036  1/16 the number of (n+1) X 7 0..3 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9·2^(n/2-1)+9·2^(n-1)+310 if n is even
 *            9·2^(n-1)+3·2^((n+1)/2)+310 if n is odd (OEIS A184036)
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t result;
    if ((n & 1) == 0)       /* even n */
    {
        seq_t p1 = pow2((seq_t)2, n / 2 - 1); /* 2^(n/2-1) */
        seq_t p2 = pow2((seq_t)2, n - 1);     /* 2^(n-1)   */
        result = (seq_t)9 * p1 + (seq_t)9 * p2 + (seq_t)310;
    }
    else                   /* odd n */
    {
        seq_t p1 = pow2((seq_t)2, n - 1);          /* 2^(n-1)   */
        seq_t p2 = pow2((seq_t)2, (n + 1) / 2);    /* 2^((n+1)/2) */
        result = (seq_t)9 * p1 + (seq_t)3 * p2 + (seq_t)310;
    }
    return result;
}
