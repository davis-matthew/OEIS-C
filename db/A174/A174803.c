/*
 * A174803  a(n) = n + ceiling(sqrt(n))*floor(sqrt(n)).
 * https://oeis.org/A174803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(n))*ceil(sqrt(n))
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t _floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0 || mid <= n / mid) {       /* mid*mid <= n without overflow */
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
    idx_t k = _floor_sqrt_u64(n);               /* floor(sqrt(n)) */
    seq_t k_seq = (seq_t)k;
    seq_t sq     = k_seq * k_seq;               /* k*k */
    seq_t n_seq  = (seq_t)n;

    if (sq == n_seq)                           /* perfect square */
        return n_seq + sq;
    else                                        /* not a perfect square */
        return n_seq + sq + k_seq;
}
