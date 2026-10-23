/*
 * A045901  Group the natural numbers into blocks: B_1 = 1, B_2 = 2,3,4, B_3 =
 *          5,6,7,8,9, ..., each block ending in a square. Permute each block
 *          B_k by beginning with the central term, followed by the transposed
 *          symmetric pairs from B_k.
 * https://oeis.org/A045901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let k = floor_sqrt_u64(n-1)+1 // block number Let p = n -
 *            (k-1)^2 // position within block (1 .. 2k‑1) Then a(n) = k^2 - k
 *            + 1 + ((p%2==0)? (p/2) : -(p/2))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = 0xffffffffULL;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    idx_t k = floor_sqrt_u64(n - 1) + 1;
    idx_t prev_sq = (k - 1) * (k - 1);
    idx_t p = n - prev_sq;
    seq_t base = (seq_t)k * (seq_t)k - (seq_t)k + (seq_t)1;
    seq_t half = (seq_t)(p >> 1);   /* floor(p/2) */

    if (p & 1)          /* odd position: subtract */
        return base - half;
    else                /* even position: add */
        return base + half;
}
