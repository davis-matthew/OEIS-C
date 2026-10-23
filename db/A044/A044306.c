/*
 * A044306  Numbers n such that string 6,1 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains61
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the base‑9 representation of n contains the substring 61 while the base‑9 representation of n−1 does not. */
/* Implementation: Brute‑force search using only integer arithmetic. */

#include <stdbool.h>
#include "../utils/contains61.h"

seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t num   = (seq_t)1;            /* current candidate */
    const seq_t one = (seq_t)1;         /* constant for increment and subtraction */

    while (true) {
        /* Condition: “61” appears in n but not in n−1. */
        if (contains61(num) && !contains61(num - one)) {
            ++count;
            if (count == n) {
                return num;
            }
        }
        num = num + one;                /* move to next integer */
    }
}
