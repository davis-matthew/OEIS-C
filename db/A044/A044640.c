/*
 * A044640  Numbers k such that string 0,8 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has08
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the base‑9 representation of k contains the
 * substring "08" while the base‑9 representation of k+1 does not.
 * The sequence is defined by searching for such k in increasing order. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has08.h"

seq_t a(idx_t n)
{
    seq_t k = 0;      /* current candidate value          */
    idx_t count = 0;  /* number of valid terms found so far */

    while (1) {
        k++;                               /* next integer to test     */
        if (has08(k) && !has08(k + 1)) {
            count++;                       /* we found a term of the sequence */
            if (count == n)
                return k;                  /* the n‑th term is found              */
        }
    }
}
