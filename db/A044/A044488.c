/*
 * A044488  Numbers n such that string 0,5 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has05
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the base‑6 representation of n contains
   the substring "05" but the base‑6 representation of n+1 does not. */
#include <stdbool.h>
#include "../utils/has05.h"

seq_t a(idx_t n)
{
    idx_t count = 0;                  /* found numbers with the property   */
    seq_t cur   = 0;                  /* current number being inspected    */
    const seq_t one = 1;              /* small constant for increment       */

    while (count < n) {
        cur = cur + one;              /* cur = cur + 1                             */
        if (has05(cur) && !has05(cur + one)) {
            count++;                 /* found next term */
            if (count == n) return cur;
        }
    }
    return cur; /* unreachable for valid n>=1 */
}
