/*
 * A129820  List of pairs of consecutive nonprime odd numbers {m-1,m+1}.
 * https://oeis.org/A129820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A129820: List of odd composite numbers that appear in a pair of
 * consecutive nonprime odd numbers {m-1,m+1}.  The sequence is produced
 * by scanning even integers m; if both m-1 and m+1 are composite, the two
 * numbers are emitted in order.  Duplicate entries occur when an odd
 * composite has composite neighbours on both sides.  Example: 91,93,93,95.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* index must be >=1 for OEIS offset 1 */

    idx_t count = 0;            /* number of sequence elements produced so far */
    idx_t m = 4;                /* current even integer to test */

    while (count < n)
    {
        /* check if both odd neighbours of m are composite */
        bool c1 = !is_prime((idx_t)(m - 1));
        bool c2 = !is_prime((idx_t)(m + 1));
        if (c1 && c2)
        {
            /* emit m-1 */
            count++;
            if (count == n)
                return (seq_t)(m - 1);

            /* emit m+1 */
            count++;
            if (count == n)
                return (seq_t)(m + 1);
        }
        m += 2;   /* next even integer */
    }

    /* control should never reach here */
    return 0;
}
