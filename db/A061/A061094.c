/*
 * A061094  The alternating group A_n contains an element x which is not
 *          conjugate to its inverse (equivalently not all the entries in the
 *          character table of A_n are real numbers).
 * https://oeis.org/A061094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer that is not in the set
 *            {1,2,5,6,10,14}. Equivalently, a(n) is the n-th integer >= 3
 *            that avoids these four excluded values. This follows from the
 *            fact that Aₙ has a non‑real irreducible character iff n ∉
 *            {1,2,5,6,10,14}
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_excluded
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_excluded.h"

seq_t a(idx_t n)
{
    /* Starting point: the first term is 3. */
    idx_t  count = 1;          /* how many valid numbers have been found   */
    seq_t  val   = (seq_t)3;   /* current candidate value                  */

    if (n == (idx_t)1)
        return val;

    /* Find the n-th valid integer by simple enumeration.  The
       excluded set is so small that at most a few extra iterations
       are performed for any n.  For n up to about 10 000 this is
       far below the time limit. */
    while (count < n)
    {
        val++;                          /* consider next integer           */
        if (is_excluded(val))
            continue;                   /* skip excluded values            */
        count++;                        /* we found another term           */
    }
    return val;
}
