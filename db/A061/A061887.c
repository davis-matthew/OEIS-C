/*
 * A061887  n + largest square less than or equal to n; numbers in the range
 *          [2k^2,2k^2+2k] for some k.
 * https://oeis.org/A061887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(n))^2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;
    idx_t res = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {          /* 0 * 0 == 0 ≤ n always */
            res = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {    /* mid*mid ≤ n */
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t k = isqrt_u64(n);
    seq_t kk = (seq_t)k * (seq_t)k;
    return (seq_t)n + kk;
}
