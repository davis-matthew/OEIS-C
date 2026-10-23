/*
 * A049980  a(n) is the number of arithmetic progressions of positive
 *          integers, strictly increasing with sum n.
 * https://oeis.org/A049980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of arithmetic progressions of positive
 *            integers whose terms sum to n. Each progression of length m≥1,
 *            first term b>0 and common difference w>0 satisfies m*b +
 *            m*(m-1)*w/2 = n
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    seq_t count = 1;                /* progression of length 1: [n] */
    seq_t n_seq = n;

    for (idx_t m = 2; ; ++m) {
        seq_t d = (seq_t)m * (m - 1) / 2;   /* m(m-1)/2   */
        seq_t t = (seq_t)m + d;             /* triangular number T_m */
        if (t > n_seq) break;               /* no longer possible */

        /* maximum common difference w that keeps the first term positive */
        idx_t max_w = (n - m) / (idx_t)d;
        for (idx_t w = 1; w <= max_w; ++w) {
            seq_t val = n_seq - (seq_t)w * d;        /* n - w*d */
            if (val % (seq_t)m == 0) {
                ++count;                            /* valid (b,w,m) */
            }
        }
    }
    return count;
}
