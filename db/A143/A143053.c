/*
 * A143053  a(n) = A143051(A143051(n)).
 * https://oeis.org/A143053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n ≤ 1; for n ≥ 2 let k = floor(√(n−1))−1, block
 *            start s = (k+1)² + 1, block size m = 2k+3, shift t = m−2 = 2k+1.
 *            Then a(n) = s + ((n−s + t) mod m)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t x = 0;
    idx_t bit = (idx_t)1 << 62;          /* highest power of two ≤ 2⁶⁴ */

    while (bit > n) bit >>= 2;           /* find the largest bit ≤ sqrt(n) */
    while (bit) {
        idx_t nxt = x + bit;
        /* check nxt² ≤ n without overflow: n / nxt ≥ nxt */
        if (n / nxt >= nxt)
            x = nxt;
        bit >>= 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1u)
        return (seq_t)n;                /* a(0)=0, a(1)=1 */

    /* Determine block index k */
    idx_t m = floor_sqrt_u64(n - 1u);    /* floor(√(n‑1)) */
    idx_t k = m - 1u;                    /* k ≥ 0 for n ≥ 2 */

    /* Compute start of the block: s = (k+1)² + 1 */
    unsigned __int128 sq = (unsigned __int128)(k + 1u) * (k + 1u);
    seq_t start = (seq_t)(sq + 1u);

    /* Block size m = 2k+3, shift t = m−2 = 2k+1 */
    seq_t block_size = (seq_t)(2u * (unsigned __int128)k + 3u);
    seq_t shift      = (seq_t)(2u * (unsigned __int128)k + 1u);

    /* Compute new position within the block */
    seq_t offset      = (seq_t)n - start;
    seq_t new_offset  = offset + shift;
    if (new_offset >= block_size)
        new_offset -= block_size;

    return start + new_offset;
}
