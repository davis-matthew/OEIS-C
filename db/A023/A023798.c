/*
 * A023798  Xenodromes: all digits in base 3 are different.
 * https://oeis.org/A023798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   distinct_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) : nth number whose base‑3 representation has all different digits
   (i.e., at most one 0, one 1 and one 2).  The sequence is finite; for
   n>11 the function returns 0. */

/* The implementation checks successive integers x from 0 upward
   and keeps those whose base‑3 digits are pairwise different.  The
   first 11 such numbers are 0,1,2,3,5,6,7,11,15,19,21. */

#include <stdbool.h>
#include "../utils/distinct_base3.h"

seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many distinct‑digit numbers seen */
    for (seq_t x = 0; x < 27; ++x)   /* 3^3 = 27 is the largest possible value */
    {
        if (distinct_base3(x))
        {
            ++count;
            if (count == n)
                return x;      /* nth term found */
        }
    }
    return (seq_t)0;       /* n exceeds number of terms (only 11 exist) */
}
