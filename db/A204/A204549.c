/*
 * A204549  Symmetric matrix: f(i,j)=floor[(i+j+5)/4]-floor[(i+j+3)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the concatenation of blocks of length s-1 for each
 *            antidiagonal sum s (s ≥ 2). The block value is 1 if s mod 4 = 0
 *            or 3, otherwise 0. The block index s is determined by the
 *            triangular inequality s(s−1)/2 < n ≤ s(s+1)/2
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undefined for n < 1 */

    /* binary search for minimal m with m(m+1)/2 ≥ n   (m = s−1) */
    idx_t low = 0, high = (1ULL << 32);   /* 2^32 ≥ all needed m for 64‑bit n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low + 1;                 /* antidiagonal sum (s = m+1) */
    uint64_t r = s & 3;                /* s mod 4 */
    seq_t val = (r == 0 || r == 3) ? (seq_t)1 : (seq_t)0;
    return val;
}
