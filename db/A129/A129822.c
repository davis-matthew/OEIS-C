/*
 * A129822  Array read by rows: each row lists three consecutive nonprime odd
 *          numbers.
 * https://oeis.org/A129822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 9747, u32 n <= 9747, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth entry obtained by reading an array whose rows
   consist of three consecutive odd non‑prime numbers spaced by 2.  
   The array is formed by scanning the odd composite numbers in order
   and selecting each triplet (s, s+2, s+4) that is fully composite.
   The sequence is the row‑major traversal of that array. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Each row has 3 entries: positions 0,1,2 correspond to
       the starting composite s, s+2, s+4. */
    idx_t row = (n - 1) / 3;           /* zero‑based row number   */
    int   col = (int)((n - 1) % 3);    /* zero‑based column number*/

    idx_t count = 0;                   /* number of qualifying rows found */

    /* Scan odd composite numbers starting from 9. */
    for (idx_t i = 9; ; i += 2)
    {
        seq_t val  = (seq_t)i;         /* current odd number       */
        if (!is_prime_big(val))        /* composite check           */
        {
            seq_t val2 = val + 2;
            seq_t val4 = val + 4;
            if (!is_prime_big(val2) && !is_prime_big(val4))
            {
                if (count == row)     /* found the desired row   */
                {
                    return val + (seq_t)(2 * col);
                }
                ++count;                /* next qualifying row     */
            }
        }
    }
}
