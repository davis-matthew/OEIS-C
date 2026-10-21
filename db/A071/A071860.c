/*
 * A071860  Number of k 1<=k<=n such that sigma(k) is odd.
 * https://oeis.org/A071860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(n)) + floor(sqrt(n/2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root for a 64‑bit unsigned integer.
   Returns floor(sqrt(n)) using only integer operations. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0;
    idx_t b = 1ULL << 62;          /* largest power of four ≤ 2^64-1 */
    while (b > n)
        b >>= 2;
    while (b)
    {
        if (n >= (x + b))
        {
            n -= x + b;
            x = (x >> 1) + b;
        }
        else
        {
            x >>= 1;
        }
        b >>= 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(n)) and floor(sqrt(n/2)) using the helper above. */
    idx_t r1 = isqrt_u64(n);
    idx_t r2 = isqrt_u64(n / 2);
    return (seq_t)r1 + (seq_t)r2;
}
