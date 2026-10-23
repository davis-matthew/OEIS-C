/*
 * A174039  A triangle sequence of the
 *          form:q=2:t(n,m,q)=Sum[q^i*Floor[Eulerian[n + 1, m]/2^i], {i, 0,
 *          10}].
 * https://oeis.org/A174039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = t(row, col, 2) where t(n,m,q) = Sum_{i=0..10} q^i *
 *            Floor[ Eulerian(n+1, m) / 2^i ]
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   eulerian_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is flattened row by row (row 0 first element, etc.). */

#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* uint64_t */
#include <math.h>     /* sqrt */
#include "../utils/eulerian_number.h"

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n)
{
    /* Find the row r such that T(r) = r*(r+1)/2 ≤ n < T(r+1).  */
    idx_t r;
    if (n == 0) {
        r = 0;
    } else {
        /* Approximate with the quadratic formula and then correct. */
        double d = sqrt((double)8 * n + 1.0);
        r = (idx_t)((d - 1.0) / 2.0);

        /* Correct rounding errors. */
        while (((unsigned __int128)r * (r + 1)) / 2 > n) --r;
        while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) ++r;
    }

    /* Column index within the row. */
    idx_t prev_total = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t c = n - prev_total;

    /* Compute the Eulerian number E(r+1, c). */
    seq_t e = eulerian_number(r + 1, c);

    /* Compute the weighted sum t(r,c,2).  i runs from 0 to 10. */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i <= 10; ++i) {
        seq_t pow2 = (seq_t)(1ULL << i);
        seq_t base = e >> i;
        sum += pow2 * base;
    }

    return sum;
}
