/*
 * A044738  Numbers n such that string 2,5 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A044738:
   sequence of integers whose base‑10 representation contains
   the substring “25” but whose successor does not. */

/* NOTE: This implementation performs a simple linear search
   for each call; it does not allocate dynamic memory, so
   it works for both fixed‐width and arbitrary‑precision
   types used for seq_t and idx_t. */

#include <stdbool.h>
#include "../utils/contains25.h"

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    seq_t cur   = 0;   /* current integer being examined */
    idx_t count = 0;   /* how many qualifying numbers we have found */

    while (count < n) {
        cur += (seq_t)1;                       /* move to next integer   */

        if (contains25(cur)) {                 /* check for “25” in cur   */
            seq_t next = cur + (seq_t)1;        /* cur+1                  */
            if (!contains25(next)) {           /* cur+1 does NOT contain */
                count++;                      /* one more result found   */
            }
        }
    }

    /* When we exit the loop, cur holds the n‑th desired term. */
    return cur;
}
