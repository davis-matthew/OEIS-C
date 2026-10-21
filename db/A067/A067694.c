/*
 * A067694  Minimum number of distinct parts in a self-conjugate partition of
 *          n, or 0 if n=2.
 * https://oeis.org/A067694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n = 0 or n = 2; 1 if n is a perfect square; 3 if n
 *            ≡ 2 (mod 4) and n > 0; 2 otherwise
 * Range:     u8 n <= 9733, u16 n <= 9733, u32 n <= 9733, u64 n <= 9733,
 *            u128 n <= 9733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n)
{
    /* simple integer square‑root (supports very large n with 64‑bit arithmetic) */
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 2)
        return (seq_t)0;

    /* square? */
    idx_t r = isqrt_u64(n);
    if (r * r == n)
        return (seq_t)1;

    /* n ≡ 2 (mod 4) and n > 0 */
    if ((n & 3) == 2)
        return (seq_t)3;

    /* default */
    return (seq_t)2;
}
