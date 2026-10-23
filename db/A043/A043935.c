/*
 * A043935  Numbers k such that 0 and 4 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043935: numbers k such that the base‑8 representation of k contains
   adjacent digits 0 and 4, but the base‑8 representation of k+1 does not.
   The implementation searches sequentially for the n‑th such number. */

#include <stdbool.h>

/* Return true if x has adjacent octal digits 0 and 4 (in any order). */
static inline bool has_adjacent_04(seq_t x)
{
    /* Read the least significant octal digit. */
    uint8_t prev = (uint8_t)(x & 7);
    x >>= 3;
    while (x != 0)
    {
        uint8_t cur = (uint8_t)(x & 7);
        if ((cur == 0 && prev == 4) || (cur == 4 && prev == 0))
            return true;
        prev = cur;
        x >>= 3;
    }
    return false;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* n must be >= 1 according to OEIS offset; but we tolerate n==0. */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    seq_t k = 0;

    for (;; ++k)
    {
        if (has_adjacent_04(k) && !has_adjacent_04(k + 1))
        {
            ++count;
            if (count == n)
                return k;
        }
    }
}
