/*
 * A186288  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the squares and pentagonal
 *          numbers. Complement of A186289.
 * https://oeis.org/A186288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((1 + sqrt(1 + 24 n^2)) / 6) - (n^2 is
 *            pentagonal ? 1 : 0)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the adjusted rank of the square n^2 in the combined ordered list
   of squares and pentagonal numbers, where ties are resolved by placing the
   square before the pentagonal entry. */
#include <stdbool.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n) {
    /* compute n^2 */
    seq_t N = (seq_t)n * (seq_t)n;

    /* t = 1 + 24*N */
    seq_t t = (seq_t)1 + (seq_t)24 * N;

    /* floor(sqrt(t)) */
    seq_t root = isqrt_seq(t);

    /* number of pentagonal numbers <= N */
    seq_t pent_le = (root + (seq_t)1) / (seq_t)6;

    /* check if N itself is a pentagonal number */
    seq_t isPent = 0;
    if (root * root == t && ((root + (seq_t)1) % (seq_t)6 == 0))
        isPent = 1;

    /* rank = (# squares <= N) + (# pentagons < N) = n + pent_le - isPent */
    seq_t rank = (seq_t)n + pent_le - isPent;
    return rank;
}
