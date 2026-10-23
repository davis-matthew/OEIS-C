/*
 * A036704  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n+1/2.
 * https://oeis.org/A036704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A057655(n^2+n)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* = number of Gaussian integers z = a+bi with |z| ≤ n + 1/2 */
/* = number of integer pairs (a,b) with a^2 + b^2 ≤ n^2 + n */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* m = n^2 + n */
    seq_t m = (seq_t)n * ((seq_t)n + (seq_t)1);

    /* max_a = floor(sqrt(m)) */
    seq_t max_a_seq = floor_sqrt(m);
    idx_t max_a = (idx_t)max_a_seq;   /* fits in idx_t (≤ ~1e4 for n≤1e4) */

    seq_t count = 0;
    for (idx_t a = 0; a <= max_a; ++a) {
        seq_t a_sq = (seq_t)a * (seq_t)a;
        seq_t bmax = floor_sqrt(m - a_sq);
        seq_t pairs = (seq_t)2 * bmax + (seq_t)1;

        if (a == 0)
            count += pairs;
        else
            count += (seq_t)2 * pairs;   /* for +a and -a */
    }
    return count;
}
