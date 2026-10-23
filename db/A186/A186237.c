/*
 * A186237  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers
 *          (A000217) and heptagonal numbers (A000566). Complement of A186238.
 * https://oeis.org/A186237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((sqrt(9 + 40 * n(n+1)/2) + 3) / 10)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Adjusted joint rank sequence of triangular (A000217) and heptagonal (A000566) numbers. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; behavior for 0 is irrelevant */
        return 0;

    /* triangular number T_n = n(n+1)/2 */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* temporary value 9 + 40*T_n */
    seq_t tmp = 9 + 40 * tn;

    /* integer square root of tmp */
    seq_t sp = floor_sqrt(tmp);

    /* number of heptagonal numbers <= T_n */
    seq_t hcnt = (sp + 3) / 10;

    /* adjusted joint rank */
    return (seq_t)n + hcnt;
}
