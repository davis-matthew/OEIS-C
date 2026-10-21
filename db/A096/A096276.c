/*
 * A096276  Number of partitions of n with a product <=n.
 * https://oeis.org/A096276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n whose product of parts <= n.
 *            Computation: depth‑first search over non‑increasing parts with
 *            product bound. The search is efficient because the product bound
 *            shrinks rapidly
 * Range:     u8 n <= 79, u16 n <= 5507, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */

/* recursive helper: count partitions of 'rem' with largest part <= 'max'
   where the product of the remaining parts must not exceed 'limit'.
   rem   : remaining sum to partition
   max   : maximum allowed part (non‑increasing)
   limit : maximum allowed product for the rest of the parts
*/
static seq_t rec(idx_t rem, idx_t max, idx_t limit)
{
    if (rem == 0)          /* empty partition of the remainder */
        return (seq_t)1;

    if (limit == 0)        /* impossible, should not happen with correct calls */
        return (seq_t)0;

    if (max == 1 || limit == 1) /* only 1’s can be used */
        return (seq_t)1;        /* unique partition made of all ones */

    seq_t sum = 0;
    idx_t upper = (rem < max) ? rem : max;   /* largest part we can try */

    /* try parts from 'upper' down to 2; part 1 is handled separately */
    for (idx_t p = upper; p > 1; --p) {
        if (p > limit)               /* cannot pick this part, product would exceed */
            continue;
        idx_t new_limit = limit / p; /* product capacity for the rest */
        if (new_limit == 1) {        /* rest must be all ones → one partition */
            sum += (seq_t)1;
        } else {
            sum += rec(rem - p, p, new_limit);
        }
    }

    /* choose part 1 as the next part -> all remaining parts are ones */
    sum += (seq_t)1;

    return sum;
}

/* public interface required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* by definition a(0)=0 */
    return rec(n, n, n);
}
