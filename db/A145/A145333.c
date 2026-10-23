/*
 * A145333  Numbers n such that there exists x in N : (x+43)^3-x^3=n^2.
 * https://oeis.org/A145333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 12943
 *   a(2) = 436295587
 *   a(n) = 33710 * a(n-1) - a(n-2)   for n ≥ 3
 *
 * This implements the second-order recurrence directly.  It works for any
 * order of calls and requires only a couple of scalar values; thus it is
 * O(n) per call and respects all type constraints.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* safeguard: offset is 1, but if n == 0 return 0 */
    if (n == 0) return 0;

    /* small constants as seq_t */
    static const seq_t coeff = 33710;
    static const seq_t a1 = 12943;
    static const seq_t a2 = 436295587;

    if (n == 1) return a1;
    if (n == 2) return a2;

    seq_t prev = a1;   /* a(1)   */
    seq_t curr = a2;   /* a(2)   */
    for (idx_t i = 3; i <= n; ++i) {
        /* next = coeff * curr - prev;  no overflow on arbitrary-precision types */
        seq_t next = coeff * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
