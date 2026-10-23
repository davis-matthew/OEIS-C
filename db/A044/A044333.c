/*
 * A044333  Numbers n such that string 0,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   contains01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains01.h"

/* Numbers n such that the decimal representation of n contains the
 * substring "01" but the decimal representation of n-1 does not.
 * The function performs a linear scan, testing each integer
 * until the n-th qualifying number is found. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but safe */

    idx_t  count = 0;      /* how many qualifying numbers found */
    seq_t  num   = 1;      /* current integer to test */

    while (true) {
        if (contains01(num) && !contains01(num - 1)) {
            ++count;
            if (count == n)
                return num;
        }
        ++num;
    }
}
