/*
 * A219923  Number of ways to write n=x+y (x>0, y>0) with x-1, x+1 and 2*x*y+1
 *          all prime.
 * https://oeis.org/A219923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count representations of n = x + y with x>0, y>0, and
   x-1, x+1, 2xy+1 all prime.  a(n) = #{ (x,y) : x+y=n, x>0, y>0,
   all three numbers prime } */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* For n ≤ 2 no positive x,y exist with x+y=n and both x-1,x+1 prime. */
    if (n <= 2)
        return 0;

    seq_t count = 0;
    for (idx_t x = 2; x < n; ++x) {
        idx_t y = n - x;                     /* y > 0 */

        if (!is_prime(x - 1))
            continue;
        if (!is_prime(x + 1))
            continue;

        /* 2*x*y + 1 fits safely into 64 bits for the given bounds. */
        uint64_t val = 2ULL * x * y + 1ULL;
        if (is_prime((idx_t)val))
            ++count;
    }
    return count;
}
