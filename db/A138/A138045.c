/*
 * A138045  Triangle read by rows: largest proper divisor of n as a table,
 *          ones excluded.
 * https://oeis.org/A138045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = largest proper divisor of the row index r if the column
 *            index within the triangle equals that divisor; otherwise 0. The
 *            row r is the smallest integer with r(r+1)/2 >= n, and the column
 *            k = n - r(r-1)/2
 * Range:     u8 n <= 9896, u16 n <= 9896, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   int_sqrt_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/int_sqrt_uint128.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n  */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    uint64_t root = int_sqrt_uint128(val);
    uint64_t r = (root - 1) >> 1;          /* (root-1)/2 */
    if ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* Column index within the row (1‑based) */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    uint64_t k = n - (uint64_t)prev_tri;

    if (r <= 1) return (seq_t)0;

    /* Find largest proper divisor of r.  If r is prime, that divisor is 1. */
    uint64_t sp = 0;
    for (uint64_t p = 2; p * p <= r; ++p)
    {
        if (r % p == 0)
        {
            sp = p;
            break;
        }
    }
    uint64_t d = (sp == 0) ? 1 : r / sp;   /* largest proper divisor > 1 */

    if (d > 1 && k == d)
        return (seq_t)d;
    else
        return (seq_t)0;
}
