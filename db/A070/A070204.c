/*
 * A070204  Number of isosceles integer triangles with perimeter n having
 *          integral inradius.
 * https://oeis.org/A070204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 90, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A070204(n) = #{{ a,b ∈ ℕ | 2a+b=n , b<2a , 
 *                           4a²‑b² is a perfect square d² , 
 *                           and 2n | b·d } }
 * (Direct enumeration for all a < n/2)
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* No triangle possible for n < 3 */
    if (n < 3) return (seq_t)0;

    /* a ranges from 1 to floor((n‑1)/2) – ensures b>0. */
    idx_t max_a = (n - 1) / 2;
    seq_t count = 0;

    for (idx_t a_val = 1; a_val <= max_a; ++a_val)
    {
        idx_t b = n - 2 * a_val;
        /* Triangle inequality: b must be strictly less than 2a. */
        if (b >= 2 * a_val) continue;

        /* Compute t = 4a² – b²; it must be positive. */
        idx_t t = 4 * a_val * a_val - b * b;
        if (t == 0) continue;              /* degenerate triangle */

        seq_t tt = (seq_t)t;
        seq_t d = floor_sqrt(tt);          /* integer sqrt of t */
        if (d * d != tt) continue;         /* not a perfect square */

        /* Integral inradius test: 2n | b·d. */
        seq_t num = (seq_t)b * d;
        seq_t den = (seq_t)2 * (seq_t)n;
        if (num % den == 0) count += (seq_t)1;
    }

    return count;
}
