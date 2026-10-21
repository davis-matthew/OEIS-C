/*
 * A214901  Number of nXnXn triangular 0..3 arrays with every horizontal row
 *          nondecreasing and having the same average value.
 * https://oeis.org/A214901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + 2 * Π_{i=1}^{n} g(i) where g(i) = Σ_{c=0}^{⌊i/3⌋}
 *            (⌊(i-3c)/2⌋ + 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   count_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/count_g.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        prod *= count_g(i);
    }
    seq_t res = prod + prod;                  /* 2 * prod */
    res = res + (seq_t)2;                    /* + 2 */
    return res;
}
