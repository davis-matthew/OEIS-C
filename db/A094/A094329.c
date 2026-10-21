/*
 * A094329  Iterate the map in A006369 starting at 16.
 * https://oeis.org/A094329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 92, u32 n <= 205, u64 n <= 796,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094329:  Iterate the map in A006369 starting at 16.
   Map f(x):  if (x % 3 == 0)  f(x) = x - x/3;
              else             f(x) = x + (x + 1)/3.
   a(1) = 16, a(n) = f(a(n-1)) for n > 1.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)16;        /* a(1) = 16; a(0) or negative indices are not used */

    seq_t cur = (seq_t)16;
    for (idx_t i = 2; i <= n; ++i)
    {
        if ((cur % 3) == 0)
            cur = cur - cur / 3;          /* 2/3 of cur, computed as cur - cur/3 */
        else
            cur = cur + (cur + 1) / 3;    /* cur + floor((cur+1)/3) */
    }
    return cur;
}
