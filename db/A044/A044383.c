/*
 * A044383  Numbers n such that string 5,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A044383: Numbers n such that "51" occurs in the decimal
   representation of n but not in that of n-1.
   Implementation: linear scan from 1 upwards, keeping a counter of
   qualifying numbers until the requested index n is reached.
*/

#include <stdbool.h>   /* for bool, true, false */
#include <stdint.h>    /* for uintptr_t, if needed */

/* Forward declaration of seq_t and idx_t types:
   They will be defined in the build environment. */

static inline bool contains_51(seq_t value) {
    /* Check for substring "51" by scanning decimal digits from LSB upwards. */
    seq_t last = value % 10;
    value /= 10;
    while (value > 0) {
        seq_t d = value % 10;
        if (d == (seq_t)5 && last == (seq_t)1)
            return true;       /* found "51" */
        last = d;
        value /= 10;
    }
    return false;
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n) {
    idx_t count = 0;          /* how many qualifying numbers found */
    seq_t num = 1;            /* candidate number to test */

    /* Keep searching until we have found the n-th number. */
    while (count < n) {
        num++;                              /* move to next integer */
        if (contains_51(num) && !contains_51(num - (seq_t)1)) {
            count++;                        /* new member of the sequence */
        }
    }
    return num;              /* num is the n-th member */
}
