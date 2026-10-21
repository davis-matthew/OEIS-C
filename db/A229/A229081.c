/*
 * A229081  Numbers n such that there exists a square m^2 with 3n^2 - n <= m^2
 *          <= 3n^2 + n.
 * https://oeis.org/A229081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 876, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS sequence A229081.
   Sequence A229081: a(n) = the nth positive integer k such that
   there exists a square m² with 3k² – k ≤ m² ≤ 3k² + k.
   This implementation searches k consecutively and tests the
   interval property via an integer square‑root routine. */

static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t mid, mid2, ans = 0;
    while (low <= high) {
        mid = (low + high) / 2;
        mid2 = mid * mid;
        if (mid2 <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;          /* prevent underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for index 0, not used */
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        seq_t kk = (seq_t)k;
        seq_t k_sq = kk * kk;
        seq_t upper = 3 * k_sq + kk;                 /* 3k² + k */
        seq_t lower = 3 * k_sq - kk;                 /* 3k² – k */
        seq_t s = isqrt_seq(upper);
        if (s * s >= lower) {
            count++;
            if (count == n) return kk;
        }
        k++;
    }
}
