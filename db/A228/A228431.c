/*
 * A228431  Number of ordered ways to write n = x + y (x, y > 0) with p(3, x)
 *          + p(6, y) prime, where p(3, k) denotes the triangular number
 *          k*(k+1)/2 and p(6, k) denotes the hexagonal number k*(2*k-1) =
 *          p(3, 2*k-1).
 * https://oeis.org/A228431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (x,y) with x>0, y>0, x+y=n such
 *            that p(3,x)+p(6,y) is prime, where p(3,k)=k*(k+1)/2 and
 *            p(6,k)=k*(2*k-1)
 * Range:     u8 n <= 12, u16 n <= 355, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* n = 1 gives no positive x,y with x+y=n */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;

        seq_t tri = (seq_t)x * ((seq_t)x + 1) / 2;
        seq_t hex = (seq_t)y * ((seq_t)2 * y - 1);
        seq_t sum = tri + hex;

        if (isPrimeGeneric(sum))
            ++count;
    }

    return count;
}
