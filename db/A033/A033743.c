/*
 * A033743  Expansion of Product_{d | 31} theta_3(q^d).
 * https://oeis.org/A033743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i+j=n} c(i) * d(j) where c(i)=1 if i=0 else 2 if i is
 *            a perfect square d(j)=1 if j=0 else 2 if j/31 is a perfect
 *            square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation directly convolves the coefficient sequences of
   θ₃(q) and θ₃(q³¹).  For each perfect‑square i ≤ n we examine j=n−i;
   if j is 0 or a multiple of 31 whose quotient is a perfect square,
   we add the product of the corresponding coefficients. */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0;; ++k) {
        idx_t i = k * k;
        if (i > n)
            break;

        idx_t j = n - i;

        /* coefficient for i */
        seq_t ci = (i == 0) ? (seq_t)1 : (seq_t)2;

        seq_t cj;
        if (j == 0) {
            cj = (seq_t)1;
        } else if (j % 31 == 0) {
            idx_t t = j / 31;
            idx_t s = _floor_isqrt_u64(t);
            if (s * s == t)
                cj = (seq_t)2;
            else
                continue;        /* j not of the form 31·square   */
        } else {
            continue;            /* j not of the form 31·square   */
        }

        sum += ci * cj;
    }

    return sum;
}
