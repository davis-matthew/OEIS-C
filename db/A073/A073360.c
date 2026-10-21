/*
 * A073360  Nested floor product of n and fractions (k+1)/k for all k>0 (mod
 *          3), divided by 3.
 * https://oeis.org/A073360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1/3 * limit of the nested product a(n) = (1/3) * floor( …
 *            floor( floor( n*(2/1) )*(3/2) )*(5/4) … ) where the factors
 *            (k+1)/k are taken for all k > 0 with k ≡/≡ 0 (mod 3)
 * Range:     u8 n <= 7, u16 n <= 77, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the product stabilizes when k > current value, at which point
   floor(cur * (k+1)/k) = cur and cur / k = 0.  The final result is
   guaranteed to be divisible by 3. */

#include <stdint.h>
#include <stdbool.h>

/* seq_t may be any unsigned integer type or an arbitrary‑precision integer
   class that supports +, -, *, / with truncating division.  idx_t is an
   unsigned integer type (uint64_t).  No external libraries are used. */
seq_t a(idx_t n)
{
    seq_t cur = n;          /* current value of the nested product   */
    for (idx_t k = 1; ; ++k)  /* iterate over k = 1,2,4,5,7,8,…   */
    {
        if (k % 3 == 0)          /* skip multiples of 3              */
            continue;

        seq_t add = cur / k;     /* floor(cur/k)                      */
        if (add == 0)            /* no further change – product fixed  */
            break;

        cur += add;              /* cur = floor(cur * (k+1) / k)      */
    }

    return cur / 3;              /* the definition divides the stable value by 3 */
}
