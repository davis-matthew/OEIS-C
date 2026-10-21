/*
 * A072689  Difference between (least square >= n) and (largest square <= n).
 * https://oeis.org/A072689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a perfect square; otherwise 2*floor_sqrt(n)+1,
 *            where floor_sqrt(n) = ⌊√n⌋
 * Range:     u8 n <= 9999, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t floor_sqrt_idx(idx_t n)
{
    /* Compute ⌊√n⌋ for n up to UINT64_MAX using integer binary search. */
    idx_t low = 0;
    idx_t high = (idx_t)1ULL << 32;  /* (2^32)^2 = 2^64 > UINT64_MAX */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        /* mid*mid fits in uint64_t because mid <= 2^32-1 */
        if (mid <= n / mid) {  /* mid*mid <= n  */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;  /* high is the largest mid with mid*mid <= n */
}

seq_t a(idx_t n)
{
    idx_t k = floor_sqrt_idx(n);
    /* If n is a perfect square, result is 0. */
    if ((seq_t)k * (seq_t)k == (seq_t)n) {
        return (seq_t)0;
    }
    /* Otherwise, difference between consecutive squares: (k+1)^2 - k^2 = 2*k + 1. */
    return (seq_t)k * (seq_t)2 + (seq_t)1;
}
