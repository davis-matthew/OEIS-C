/*
 * A138798  Values of j corresponding to least possible k>0 with T(k)-T(j)=n,
 *          where T(i)>0 are the triangular numbers A000217.
 * https://oeis.org/A138798
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal j such that T(k)-T(j)=n for the smallest k>0
 *            (where T(m)=m(m+1)/2)
 * Range:     u8 n <= 9, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for seq_t (floor(sqrt(x))) */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;   /* mid = ceil((low+high)/2) */
        if (mid == 0) {
            low = mid;
            continue;
        }
        /* avoid overflow: mid <= x/mid  <=>  mid*mid <= x */
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* value for indices before the OEIS offset is undefined */

    seq_t n_seq = (seq_t)n;

    for (idx_t k = 1; ; ++k) {
        seq_t t_k = (seq_t)k * ((seq_t)k + 1) / 2;

        if (t_k <= n_seq)
            continue;  /* diff would be non‑positive */

        seq_t diff = t_k - n_seq;           /* diff > 0 */
        seq_t s = 8 * diff + 1;
        seq_t sqrt_s = isqrt_seq(s);

        if (sqrt_s * sqrt_s == s && (sqrt_s & 1)) {   /* diff is triangular */
            seq_t j = (sqrt_s - 1) / 2;
            return j;
        }
    }
}
