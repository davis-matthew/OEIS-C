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
 * Helpers:   calc_strokes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_strokes.h"

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
