/*
 * A044629  Numbers n such that string 7,5 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest integer n such that the base‑8 *
 *            representation of n contains the substring "75" but the *
 *            representation of n+1 does not
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   contains75
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/contains75.h"

seq_t a(idx_t n)
{
    /* Offset 1: undefined for n = 0, but return 0 in that case. */
    if (n == 0) return 0;
    idx_t count = 0;
    seq_t m = 1;
    while (1) {
        if (contains75(m)) {
            if (!contains75(m + 1)) {
                ++count;
                if (count == n) return m;
            }
        }
        ++m;
    }
}
