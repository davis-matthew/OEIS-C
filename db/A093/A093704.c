/*
 * A093704  Value of initial digit of n in Roman numeral representation.
 * https://oeis.org/A093704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 499, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Leading Roman numeral digit of n.  Uses the standard greedy
   construction of Roman numerals; the first symbol that is subtracted
   gives the value of a(n).  */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const idx_t thresholds[] = {
        1000ULL, 900ULL, 500ULL, 400ULL, 100ULL,
        90ULL, 50ULL, 40ULL, 10ULL, 9ULL, 5ULL, 4ULL, 1ULL
    };
    static const seq_t values[] = {
        1000, 100, 500, 100, 100,
        10, 50, 10, 10, 1, 5, 1, 1
    };
    size_t i;
    for (i = 0; i < sizeof(thresholds) / sizeof(thresholds[0]); ++i)
        if (n >= thresholds[i])
            return values[i];
    /* n >= 1 by contract, so this line is unreachable */
    return (seq_t)0;
}
