/*
 * A182774  Beatty sequence for 1+2^(-2/3).
 * https://oeis.org/A182774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1 + 2^(-2/3)))
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 2161, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_ts = (seq_t)n;
    if (n_ts == (seq_t)0) return (seq_t)0;  /* safety for n=0 */

    /* n^3 */
    seq_t n3 = n_ts * n_ts * n_ts;

    /* We need the largest k such that 4*k^3 <= n^3.
       This k equals floor(n / cbrt(4)). */
    seq_t lo = (seq_t)0;
    seq_t hi = n_ts;  /* upper bound: k < n */

    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t m3 = mid * mid * mid;
        seq_t m4 = m3 * (seq_t)4;
        if (m4 <= n3) {
            lo = mid + (seq_t)1;
        } else {
            hi = mid - (seq_t)1;
        }
    }

    /* a(n) = n + floor(n / cbrt(4)) = n + hi */
    return n_ts + hi;
}
