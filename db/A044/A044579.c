/*
 * A044579  Numbers n such that string 1,3 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: linear scan over integers, testing for the octal
   substring "13" in n and not in n+1. */
#include <stdbool.h>

/* Return true iff x in base 8 contains the substring 13 */
static bool has13(seq_t x)
{
    seq_t prev = 0;
    while (x > 0) {
        seq_t cur = x % 8;
        if (prev == 3 && cur == 1)
            return true;
        prev = cur;
        x = x / 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; safety */
    seq_t num = 1;
    idx_t found = 0;

    while (1) {
        if (has13(num) && !has13(num + 1)) {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
}
