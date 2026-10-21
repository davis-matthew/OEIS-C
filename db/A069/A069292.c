/*
 * A069292  Sum of square roots of square divisors of n <= sqrt(n).
 * https://oeis.org/A069292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k ≥ 1, k² | n, k ≤ n^{1/4}} k
 * Range:     u8 n <= 8839, u16 n <= 8839, u32 n <= 8839, u64 n <= 8839,
 *            u128 n <= 8839, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_sqrt_u64(idx_t x) {
    idx_t low = 0, high = x, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = (mid == 0) ? 0 : mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    idx_t sqrt_n   = floor_sqrt_u64(n);       /* ⌊√n⌋ */
    idx_t max_k    = floor_sqrt_u64(sqrt_n);  /* ⌊√⌊√n⌋⌋ = ⌊n¹⁄⁴⌋ */

    seq_t sum = 0;
    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t ksq = k * k;           /* k² fits in idx_t for the range des­cribed above */
        if (n % ksq == 0) {
            sum += (seq_t)k;
        }
    }
    return sum;
}
