/*
 * A044547  Numbers n such that string 4,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_40_in_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns the n-th integer m such that the base‑7 representation
 * of m contains the substring "40" while the base‑7 representation of m+1 does
 * not contain that substring.  The definition is implemented by a simple
 * brute‑force scan.
 */

#include <stdbool.h>
#include "../utils/has_40_in_base7.h"

seq_t a(idx_t n)
{
    idx_t count = 0; /* how many qualifying numbers have been found */
    seq_t value = 0; /* current candidate number */

    while (count < n) {
        ++value; /* move to the next integer */

        /* Check the condition for value and value+1 */
        if (has_40_in_base7(value) && !has_40_in_base7(value + 1))
            ++count;
    }

    return value; /* value is the n-th qualifying integer */
}
