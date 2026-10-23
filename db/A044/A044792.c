/*
 * A044792  Numbers n such that string 7,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer x such that the string "79" occurs in *
 *            decimal representation of x but not in x+1
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains79
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/contains79.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1; return 0 for safety */
    seq_t cur = 79;                     /* first term */
    if (n == 1) return cur;

    idx_t count = 1;                    /* we already found the first term */
    for (;;) {
        cur = cur + 1;                  /* move to next integer */
        if (!contains79(cur)) continue;

        seq_t nxt = cur + 1;
        if (contains79(nxt)) continue;  /* skip if next number also contains 79 */

        ++count;                        /* we have a new valid term */
        if (count == n) return cur;     /* found the requested term */
    }
}
