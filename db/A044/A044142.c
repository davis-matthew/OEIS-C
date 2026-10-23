/*
 * A044142  Numbers k such that string 0,4 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k such that the base‑7 representation
 *            of k contains the substring “04” but the base‑7 representation
 *            of k‑1 does not
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_sub04
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_sub04.h"

/* Return the n‑th term of the sequence A044142.  The sequence
   is generated on the fly; no caching or recursive calls are used. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 1, return 0 for safety */
        return 0;

    seq_t k = 0;                 /* candidate k */
    idx_t count = 0;             /* how many terms found */

    while (count < n) {
        k++;
        if (has_sub04(k) && !has_sub04(k - 1))
            count++;
    }
    return k;
}
