/*
 * A107663  a(2n) = 2*4^n-1, a(2n+1) = (2^(n+1)+1)^2; interlaces A083420 with
 *          A028400.
 * https://oeis.org/A107663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n) = 2*4^n-1, a(2n+1) = (2^(n+1)+1)^2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* power of two by repeated multiplication */
static seq_t pow2_idx(idx_t e)
{
    seq_t r = 1;
    while (e--) r *= 2;
    return r;
}

seq_t a(idx_t n)
{
    /* even index */
    if ((n & 1) == 0)
    {
        idx_t k = n >> 1;                     /* n = 2k   */
        seq_t v = pow2_idx((idx_t)(2 * k + 1));/* 2^(2k+1) */
        v -= 1;                               /* 2^(2k+1)-1 = 2*4^k-1 */
        return v;
    }
    /* odd index */
    {
        idx_t k = (n - 1) >> 1;               /* n = 2k+1 */
        seq_t v = pow2_idx(k + 1);            /* 2^(k+1)   */
        v += 1;                               /* 2^(k+1)+1 */
        return v * v;                         /* square */
    }
}
