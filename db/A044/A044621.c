/*
 * A044621  Numbers n such that string 6,5 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has65_octal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044621: numbers n such that the octal representation of n contains
 * the substring "65", but the octal representation of n+1 does not.  This function
 * scans the natural numbers, counts the qualifying numbers, and returns the
 * n-th occurrence.  It works for any build where seq_t supports the usual
 * arithmetic and comparison operators. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has65_octal.h"

seq_t a(idx_t n)
{
    if (n == 0) { /* unspecified behaviour for offset 0; return 0 */
        return 0;
    }

    idx_t count = 0;
    seq_t candidate = 1;          /* start checking from 1 */

    for (;;) {
        if (has65_octal(candidate) && !has65_octal(candidate + 1)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        candidate++;
    }
}
