/*
 * A185387  Expansion of e.g.f. exp(x)+log(1/(1-x)).
 * https://oeis.org/A185387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! + 1 for n > 0, a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t f = factorial(n - 1);
    return f + 1;
}
