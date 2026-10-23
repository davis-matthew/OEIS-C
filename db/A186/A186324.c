/*
 * A186324  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the squares and octagonal
 *          numbers. Complement of A186325.
 * https://oeis.org/A186324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((1 + floor(sqrt(1 + 3*n^2))) / 3) - (1 if n^2
 *            is an octagonal number, else 0)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isqrt_seq.h"

/* Computes value of sequence A186324 at index n (1‑based). */
seq_t a(idx_t n)
{
    /* n^2 */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* m = 1 + 3*n^2 */
    seq_t m = (seq_t)1 + (seq_t)3 * sq;

    /* floor(sqrt(m)) */
    seq_t sqrt_m = isqrt_seq(m);

    /* t = floor((1 + sqrt_m) / 3) */
    seq_t t = ((seq_t)1 + sqrt_m) / 3;

    /* test if n^2 is an octagonal number */
    bool is_oct = false;
    if (sqrt_m * sqrt_m == m) {
        /* integer sqrt exists */
        seq_t num = (seq_t)1 + sqrt_m;
        if (num % (seq_t)3 == (seq_t)0) {
            /* j = num / 3 is the index of the octagonal number equal to n^2 */
            is_oct = true;
        }
    }

    if (is_oct) {
        t -= (seq_t)1;
    }

    return (seq_t)n + t;
}
