/*
 * A195176  a(n) = 3*n - floor(n*sqrt(2)).
 * https://oeis.org/A195176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - floor(n*sqrt(2))
 * Range:     u8 n <= 11, u16 n <= 509, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by evaluating floor(sqrt(2*n^2)) using integer arithmetic. */

static seq_t floor_sqrt_seq(seq_t x)
{
    if (x == 0) return 0;          /* sqrt(0) = 0 */

    seq_t low = 0;
    seq_t high = 1;

    /* Find an upper bound: high is the smallest power of two > sqrt(x) */
    while (high <= x / high) {
        high <<= 1;                 /* high = high * 2 */
    }

    low = high >> 1;                /* low is half of high */

    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        if (mid <= x / mid) {      /* mid*mid <= x without overflow */
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t m  = nn + nn;                     /* 2*n^2 */
    seq_t s  = floor_sqrt_seq(m);           /* floor(sqrt(2*n^2)) = floor(n*sqrt(2)) */
    seq_t three_n = (seq_t)3 * (seq_t)n;     /* 3*n */
    return three_n - s;                     /* a(n) = 3*n - floor(n*sqrt(2)) */
}
