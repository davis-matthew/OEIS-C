/*
 * A120769  Starting from a(0)=1, recursively a(2^k+r) = (2^k-r)*a(2^k-1-r),
 *          0<=r < 2^(k+1).
 * https://oeis.org/A120769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(0) = 1; for n > 0, write n = 2^k + r with 0 ≤ r < 2^k, then
 *            a(n) = (2^k - r) * a(2^k - 1 - r)
 * Range:     u8 n <= 20, u16 n <= 81, u32 n <= 340, u64 n <= 5416,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t _floor_log2_u64(idx_t x)
{
    /* assume x > 0 */
    idx_t r = 0;
    while (x >> 1)
    {
        x >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* find the highest power of two not exceeding n */
    idx_t k  = _floor_log2_u64(n);
    idx_t pow = (idx_t)1 << k;          /* 2^k */
    idx_t r   = n - pow;                /* r in [0, 2^k-1] */
    idx_t m   = pow - 1 - r;            /* 2^k - 1 - r  < n */

    seq_t val = a(m);                   /* recursive call on a smaller index */
    return (seq_t)(pow - r) * val;      /* (2^k - r) * a(2^k - 1 - r) */
}
