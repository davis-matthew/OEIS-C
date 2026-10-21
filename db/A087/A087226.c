/*
 * A087226  LCM of terms in Collatz (3x+1) function initiated at n.
 * https://oeis.org/A087226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 24, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: LCM of terms in Collatz (3x+1) sequence starting at n */
/*   a(n) = lcm( n, Collatz(n-1), Collatz(n-2), … , 1 )                 */

#include "../utils/gcd.h"
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* n should be >= 1, guard clause */

    seq_t lcm = 1;                  /* current LCM of processed terms   */
    uint64_t cur = (uint64_t)n;     /* current term in the Collatz chain */

    while (1)
    {
        seq_t cur_seq = (seq_t)cur;          /* cast to seq_t                 */
        seq_t g = gcd(lcm, cur_seq);        /* greatest common divisor       */
        lcm = lcm * cur_seq / g;            /* update LCM                     */

        if (cur == 1) break;                /* reached the terminating 1     */

        /* next value of the Collatz sequence */
        if (cur & 1)                          /* odd */
            cur = cur * 3 + 1;
        else                                 /* even */
            cur >>= 1;
    }

    return lcm;
}
