/*
 * A238598  Largest integer k such that n >= k^2-k-1 = A165900(k).
 * https://oeis.org/A238598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + sqrt(4*n+5))/2) - [n==1]
 * Range:     u8 n <= 6, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t result = 0;

    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t sq = mid * mid;
        if (sq <= x) {
            result = mid;
            low = mid + 1;
        } else {
            /* mid is > 0 here, otherwise we would have exited earlier */
            high = mid - 1;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute 4*n + 5 using seq_t to avoid overflow */
    seq_t val = (seq_t)n * 4 + 5;

    /* Integer square root of the value */
    seq_t sqrt_val = _isqrt_seq(val);

    /* Main formula */
    seq_t k = (sqrt_val + 1) / 2;

    /* Correct the lone special case described in the OEIS entry */
    if (n == 1) {
        k -= 1;
    }

    return k;
}
