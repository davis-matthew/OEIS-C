/*
 * A230375  Squarefree numbers congruent to 2 or 3 mod 4.
 * https://oeis.org/A230375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth squarefree integer congruent to 2 or 3 modulo 4
 * Range:     u8 n <= 106, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t count = 0;
    idx_t num   = 2;          /* start from the first positive integer ≡ 2 or 3 (mod 4) */

    while (1) {
        /* Check modulo 4: 2 or 3 */
        if ((num & 3u) == 2u || (num & 3u) == 3u) {
            if (is_squarefree((seq_t)num)) {
                ++count;
                if (count == n)
                    return (seq_t)num;
            }
        }
        ++num;
    }
}
