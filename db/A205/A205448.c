/*
 * A205448  Ordered differences of even-indexed Fibonacci numbers.
 * https://oeis.org/A205448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_{2i} - F_{2j} * where i is the least integer satisfying
 *            (i-1)*i/2 ≥ n, * and j = n - (i-2)*(i-1)/2
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   fib, find_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include "../utils/fib.h"
#include "../utils/find_i.h"

seq_t a(idx_t n)
{
    assert(n >= 1);

    idx_t i = find_i(n);
    /* Number of terms before the block with index i. */
    idx_t prev_terms = ((unsigned __int128)(i - 2) * (i - 1)) / 2;
    idx_t j = n - prev_terms;   /* 1 ≤ j < i */

    idx_t fi = 2 * i;
    idx_t fj = 2 * j;

    seq_t s_i = fib(fi);
    seq_t s_j = fib(fj);

    return s_i - s_j;
}
