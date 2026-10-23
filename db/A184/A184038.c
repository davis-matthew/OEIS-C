/*
 * A184038  1/16 the number of (n+1) X 9 0..3 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1225; for even n ≥ 2: a(n) = 9*2^(n/2-1)+9*2^(n-1)+1210;
 *            for odd n ≥ 3: a(n) = 9*2^(n-1)+3*2^((n+1)/2)+1210
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow2(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        r <<= 1;           /* shift left by 1 bit repeatedly */
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1225;

    if ((n & (idx_t)1) == (idx_t)0) {           /* even n */
        idx_t e1 = (n / (idx_t)2) - (idx_t)1;   /* n/2 - 1 */
        idx_t e2 = n - (idx_t)1;                /* n - 1 */
        seq_t t1 = (seq_t)9 * pow2(e1);
        seq_t t2 = (seq_t)9 * pow2(e2);
        return t1 + t2 + (seq_t)1210;
    } else {                                    /* odd n ≥ 3 */
        idx_t e1 = n - (idx_t)1;                /* n - 1 */
        idx_t e2 = (n + (idx_t)1) / (idx_t)2;   /* (n+1)/2 */
        seq_t t1 = (seq_t)9 * pow2(e1);
        seq_t t2 = (seq_t)3 * pow2(e2);
        return t1 + t2 + (seq_t)1210;
    }
}
