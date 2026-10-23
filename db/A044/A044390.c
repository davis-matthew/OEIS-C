/*
 * A044390  Numbers n such that string 5,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_58
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044390: a(n) is the nth integer such that its decimal representation
   contains the substring "58" whereas the integer n-1 does not. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/contains_58.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)       /* undefined for n = 0, but harmless */
        return (seq_t)0;

    seq_t num = 57;   /* integer examined last; start before first term */
    idx_t count = 0;  /* how many valid numbers found so far */

    while (1) {
        ++num;        /* next integer to test */
        if (contains_58(num) && !contains_58(num - 1)) {
            ++count;
            if (count == n)
                return num;
        }
    }
}
