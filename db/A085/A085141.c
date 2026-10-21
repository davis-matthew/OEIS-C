/*
 * A085141  Greatest nonnegative integer k such that k(3k+1)/2 <= n.
 * https://oeis.org/A085141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((sqrt(24*n+1)-1)/6)
 * Range:     u8 n <= 10, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq_isqrt(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)0;
    seq_t low  = (seq_t)0;
    seq_t high = x;
    while (low <= high) {
        /* mid = low + floor((high - low) / 2) */
        seq_t mid = low + ((high - low) >> 1);
        /* Check if mid*mid <= x without overflow:
           mid <= x / mid  <=>  mid*mid <= x   (mid > 0) */
        if (mid != (seq_t)0 && mid <= x / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Compute 24*n + 1, staying in seq_t */
    seq_t t = (seq_t)n * 24 + 1;
    /* Integer square root of t */
    seq_t s = seq_isqrt(t);
    /* floor((s-1)/6) */
    return (s - 1) / 6;
}
