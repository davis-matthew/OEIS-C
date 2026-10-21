/*
 * A078593  Number of pairs (x,y) 1<=x<=y<=n such that 1/x+1/y+1/n < 1/2.
 * https://oeis.org/A078593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 - 3*n - 8)/2 for n >= 43, otherwise a(n) is computed
 *            by brute force enumeration
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For small n, compute directly. */
    if (n < 43) {
        seq_t n_s = (seq_t)n;
        seq_t cnt = 0;
        for (idx_t x = 1; x <= n; ++x) {
            seq_t xs = (seq_t)x;
            for (idx_t y = x; y <= n; ++y) {
                seq_t ys = (seq_t)y;
                seq_t xy = xs * ys;
                seq_t left = (seq_t)2 * n_s * (xs + ys) + (seq_t)2 * xy;
                if (left < xy * n_s)
                    ++cnt;
            }
        }
        return cnt;
    }

    /* Use the formula for n >= 43. */
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn - (seq_t)3 * nn - (seq_t)8;
    return val / 2;
}
