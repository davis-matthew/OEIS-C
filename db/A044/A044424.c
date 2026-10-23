/*
 * A044424  Numbers n such that string 9,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains92
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): nth positive integer whose decimal representation contains "92" but n-1 does not. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/contains92.h"

/* Return the n‑th term of the sequence (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0)                           /* offset is 1, but behaviour for n<1 is irrelevant */
        return (seq_t)0;

    idx_t count   = 0;                     /* how many qualifying numbers have been seen   */
    idx_t candidate = 1;                   /* current integer to test                       */

    for (;;) {                             /* we know the loop will terminate                 */
        seq_t val = (seq_t)candidate;
        if (contains92(val) && !contains92(val - (seq_t)1)) {
            ++count;
            if (count == n)                /* desired term found */
                return val;
        }
        ++candidate;
    }
}
