/*
 * A176814  The number of iterations needed to reach 1 under the map n->
 *          n-bigomega(n).
 * https://oeis.org/A176814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 788, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Computes a(n) as the number of iterations required to reach
   1 under the map f(m) = m - bigomega(m), where bigomega(m) is the
   total number of prime factors of m (counted with multiplicity). */
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t steps = 0;
    idx_t cur = n;
    while (cur > 1) {
        steps++;
        seq_t w = bigomega(cur);
        cur -= (idx_t)w;   /* w is always <= cur for cur >= 2 */
    }
    return steps;
}
