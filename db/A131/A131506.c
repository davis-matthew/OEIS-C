/*
 * A131506  2n+1 appears 2n-1 times.
 * https://oeis.org/A131506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*ceil(sqrt(n)) + 1
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find floor(sqrt(n)) for an unsigned 64‑bit integer n.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static idx_t _isqrt_idx(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    idx_t floor = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= n) {
            floor = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return floor;
}

seq_t a(idx_t n)
{
    /* Floor of sqrt(n). */
    idx_t floor = _isqrt_idx(n);

    /* Ceil of sqrt(n) = floor if n is a perfect square, else floor+1. */
    idx_t ceil = floor;
    if ((unsigned __int128)floor * floor < n)
        ceil = floor + 1;

    /* Return the odd number 2*ceil + 1. */
    return (seq_t)ceil * 2 + 1;
}
