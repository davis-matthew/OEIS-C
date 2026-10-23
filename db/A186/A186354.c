/*
 * A186354  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=3i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186355.
 * https://oeis.org/A186354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((sqrt(1 + 24 n) - 1)/2) - [3n is triangular]
 * Range:     u8 n <= 2, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 – a(0) is unused */

    seq_t n_val = (seq_t)n;

    /* d = 1 + 24*n  (computed in seq_t to avoid overflow) */
    seq_t d = (seq_t)24 * n_val + 1;

    /* k = floor((sqrt(d) - 1)/2)  – the greatest j with j(j+1)/2 <= 3n */
    seq_t sqrt_d = floor_sqrt(d);
    seq_t k = (sqrt_d - 1) / 2;

    /* Check whether 3n itself is triangular (i.e., equals j(j+1)/2) */
    seq_t tri = k * (k + 1) / 2;
    seq_t three_n = n_val * 3;
    bool is_triangular = (tri == three_n);

    /* a(n) = n + k - (3n triangular ? 1 : 0) */
    return n_val + k - (is_triangular ? 1 : 0);
}
