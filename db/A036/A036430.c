/*
 * A036430  Number of iterations needed to reach 1 under the map n ->
 *          Omega(n).
 * https://oeis.org/A036430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(bigomega(n)) + 1 for n > 1, a(1)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of iterations needed to reach 1 under the map n -> Ω(n) */

#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0, a(0) undefined but returns 0 */
    idx_t cur = n;
    seq_t steps = 0;
    while (cur > 1)
    {
        cur = (idx_t)bigomega(cur); /* Ω(cur) fits comfortably in idx_t */
        steps++;
    }
    return steps;
}
