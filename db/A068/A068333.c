/*
 * A068333  Product(n/k - k) where the product is over the divisors k of n and
 *          where 1 <= k <= sqrt(n).
 * https://oeis.org/A068333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ∏_{k | n, 1 ≤ k ≤ ⌊√n⌋} (n/k − k)
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 119, u64 n <= 359,
 *            u128 n <= 1679, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root for unsigned 64‑bit numbers with no floating point */
static inline uint64_t _isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    for (int i = 31; i >= 0; --i)          /* 2^31 > sqrt(2^63-1) */
    {
        uint64_t b = r + (1ULL << i);
        if (b <= x / b)                    /* avoid overflow of b*b   */
            r = b;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* for safety; a(0) is not used      */
        return 0;

    seq_t prod = 1;
    uint64_t lim = _isqrt_u64(n);   /* n fits in uint64_t (idx_t)       */

    for (uint64_t k = 1; k <= lim; ++k)
    {
        if (n % k == 0)
        {
            seq_t factor = (seq_t)(n / k) - (seq_t)k;
            if (factor == 0)          /* product becomes zero for a square */
                return 0;
            prod *= factor;
        }
    }
    return prod;
}
