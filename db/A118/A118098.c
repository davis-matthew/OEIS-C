/*
 * A118098  Numbers that require three strokes when written in Roman numerals
 *          (excluding serifs). III IV VI IX XI LI XC CII CV CX CL.
 * https://oeis.org/A118098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, base
 * Formula:   a(n) = the nth positive integer whose Roman numeral
 *            representation requires exactly 3 strokes * (using the stroke
 *            rules: I=1, V=2, X=2, L=2, C=1, D=2, M=1; and the pattern
 *            repeats for higher * magnitudes, treating overlined symbols with
 *            the same stroke counts as their base letters)
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: full, pass, 16 terms through n = 16 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned calc_strokes(idx_t val)
{
    unsigned strokes = 0;
    idx_t pos = 0;            /* position of the current decimal digit (0 = units) */
    while (val > 0)
    {
        unsigned digit = (unsigned)(val % 10);
        if (digit)
        {
            const unsigned a5 = 2;                                   /* V-like symbols always 2 strokes */
            const unsigned a1 = (pos % 3 == 1) ? 2 : 1;               /* X-like or I/C/M like symbols */
            const unsigned a10 = (pos % 3 == 0) ? 2 : 1;              /* X-like or C/M like symbols */

            switch (digit)
            {
                case 1: strokes += a1; break;
                case 2: strokes += a1 + a1; break;
                case 3: strokes += a1 + a1 + a1; break;
                case 4: strokes += a1 + a5; break;
                case 5: strokes += a5; break;
                case 6: strokes += a5 + a1; break;
                case 7: strokes += a5 + a1 + a1; break;
                case 8: strokes += a5 + a1 + a1 + a1; break;
                case 9: strokes += a1 + a10; break;
            }
        }
        val /= 10;
        ++pos;
    }
    return strokes;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t i = 1; ; ++i)
    {
        if (calc_strokes(i) == 3)
        {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
}
