/*
 * A043211  Numbers k such that 5 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer k such that 5 and 7 occur
 *            juxtaposed (in either order) in the base‑9 representation of k,
 *            but not in the base‑9 representation of k‑1
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return true if x in base 9 contains the adjacent digit pair 5,7
   or 7,5. */
static bool has_pair(seq_t x)
{
    int last = -1;
    while (x > 0)
    {
        int d = (int)(x % 9);
        if ((last == 5 && d == 7) || (last == 7 && d == 5))
            return true;
        last = d;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but avoid UB */

    seq_t k = 1;
    idx_t count = 0;

    for (;; ++k)
    {
        if (has_pair(k) && !has_pair(k - 1))
        {
            ++count;
            if (count == n)
                return k;
        }
    }
}
