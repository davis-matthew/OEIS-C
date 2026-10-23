/*
 * A044804  Numbers m such that string 9,1 occurs in the base 10
 *          representation of m but not of m+1.
 * https://oeis.org/A044804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_91
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A044804:  
   numbers m such that the decimal representation of m contains the substring "91" but m+1 does not. */
#include <stdbool.h>
#include "../utils/has_91.h"

/* External interface: a(idx_t n) returns the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset is 1 – below 1 the value is undefined */

    idx_t   seen = 0;                  /* how many terms have been found so far          */
    seq_t   m    = 0;                  /* current integer being tested                  */

    while (seen < n)
    {
        m += (seq_t)1;                 /* examine next integer                          */
        if (has_91(m) && !has_91(m + (seq_t)1))
            ++seen;                    /* found the next term of the sequence           */
    }
    return m;                          /* m is the n‑th term                            */
}
