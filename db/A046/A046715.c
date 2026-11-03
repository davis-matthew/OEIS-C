/*
 * A046715  Secondary root edges in doubly rooted tree maps with n edges.
 * https://oeis.org/A046715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n) * binomial(2*n + 1, n - 1), with a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 247 (2026-11-01)
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
    {
        return (seq_t)0;
    }
    seq_t cat = catalan(n);
    seq_t bin = binomial(2 * n + 1, n - 1);
    return cat * bin;
}
