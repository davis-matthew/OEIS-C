/*
 * A044207  Numbers n such that string 2,4 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has24oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044207: numbers n such that the octal representation of n contains
   the substring "24" but n-1 does not. */

#include <stdbool.h>
#include "../utils/has24oct.h"

/* Return the nth term of A044207 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* only defined for n>=1, but guard against misuse */

    seq_t cur = 1;                  /* candidate integer being tested */
    idx_t count = 0;                /* how many valid terms found so far */

    while (count < n) {
        if (has24oct(cur) && !has24oct(cur - 1)) {
            count++;                /* cur is a term */
            if (count == n)         /* the required term found */
                return cur;
        }
        cur++;                      /* move to next integer */
    }
    return cur; /* unreachable, but keeps the compiler happy */
}
