/*
 * A044194  Numbers n such that string 0,7 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the substring "07" occurs in the base‑8 *
 *            representation of n but not in that of n‑1. * Computation:
 *            brute‑force scan incrementally until the n‑th such * number is
 *            found
 * Range:     u8 n <= 3, u16 n <= 1142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9735 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int has_sub07(const seq_t val)
{
    /* Detect: digits ... 0,7 ... in base 8 (most significant first) */
    idx_t v = (idx_t)val;
    int last = -1;                 /* previous (less significant) digit */
    while (v > 0)
    {
        int digit = v & 7;         /* v % 8  */
        v >>= 3;                  /* v /= 8  */
        if (last == 7 && digit == 0) /* pattern 7 (lower) followed by 0 */
            return 1;
        last = digit;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;   /* number of terms found so far */
    idx_t x = 0;       /* current integer to test */

    for (;;)
    {
        ++x;
        if (has_sub07(x) && !has_sub07(x - 1))
        {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
