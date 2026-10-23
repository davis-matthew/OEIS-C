/*
 * A044365  Numbers n such that string 3,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains33
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044365: numbers n such that "33" occurs in the decimal
   representation of n but not in n-1.  This implementation
   brute‑forces the sequence each time a call is made, which is fast
   enough for n up to about 10 000. */
#include <stdbool.h>
#include "../utils/contains33.h"

/* Return the nth term of A044365 (n >= 1).  The sequence is
   indexed as a(1) = 33, a(2) = 133, etc. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many suitable numbers found */
    idx_t num   = 0;          /* current integer to test */

    while (count < n) {
        ++num;                /* test next integer */
        if (contains33(num) && !contains33(num-1)) {
            ++count;
        }
    }
    return (seq_t)num;       /* cast to the output integer type */
}
