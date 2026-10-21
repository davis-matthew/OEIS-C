/*
 * A069161  Numbers n such that no group of order n can be a central factor.
 * https://oeis.org/A069161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   include_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069161 (OEIS):                 */
/* Numbers n such that no group of order n   */
/* can be a central factor.                 */
/* Computation: scan odd integers, skip 2   */
/* except for a(1), and include only those */
/* that are not (a) odd prime powers with   */
/* exponent > 1, nor (b) having two distinct*/
/* prime divisors p<q with p|(q-1).         */
/* a(1) = 2.                                */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/include_odd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n=0 */

    /* a(1) = 2 */
    if (n == 1)
        return (seq_t)2;

    idx_t count = 1;            /* we already have the term 2 */
    uint64_t candidate = 3;     /* start checking from the first odd integer ≥3 */

    while (count < n) {
        if (include_odd(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2;         /* only odd numbers */
    }

    return (seq_t)0;            /* should never reach here for valid n */
}
