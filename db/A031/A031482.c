/*
 * A031482  Numbers whose base-7 representation has the same number of 0's as
 *          6's.
 * https://oeis.org/A031482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   balanced06
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031482: Numbers whose base‑7 representation has equal number of 0's and 6's */

#include <stdbool.h>
#include "../utils/balanced06.h"

/* return the n‑th term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safe fallback for undefined n=0 */

    idx_t count = 0;      /* how many valid numbers found so far */
    seq_t x = (seq_t)1;   /* current number to test */

    while (count < n)
    {
        if (balanced06(x))
        {
            ++count;
            if (count == n) return x;
        }
        ++x;               /* next integer to test */
    }

    /* unreachable but keeps the compiler happy */
    return (seq_t)0;
}
