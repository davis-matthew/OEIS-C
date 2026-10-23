/*
 * A116869  Sixth column of triangle A028364.
 * https://oeis.org/A116869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..5} Catalan(k) * Catalan(n+5-k)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= 5; ++k)
    {
        seq_t left  = catalan(k);
        seq_t right = catalan(n + 5 - k);
        sum += left * right;
    }
    return sum;
}
