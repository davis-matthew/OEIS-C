/*
 * A089269  Squarefree numbers congruent to 1 or 2 mod 4.
 * https://oeis.org/A089269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for squarefree numbers ≡ 1 or 2 (mod 4). */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (seq_t x = 1;; ++x)
    {
        /* keep only numbers that are 1 or 2 modulo 4 */
        if ((x & 3) != 1 && (x & 3) != 2)
            continue;
        /* test squarefreeness */
        if (is_squarefree(x))
        {
            ++count;
            if (count == n)
                return x;
        }
    }
}
