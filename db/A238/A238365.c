/*
 * A238365  Number of dots needed to express n as a Roman numeral in Braille.
 * https://oeis.org/A238365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, base
 * Formula:   a(n) = sum of Braille dot counts of the Roman‑numeral letters of
 *            n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1u) return 0;
    seq_t result = 0;
    /*  Roman numeral values in descending order and the corresponding
        total number of Braille dots for the symbol (single or subtractive). */
    static const idx_t  values[] = {1000, 900, 500, 400, 100,  90,  50, 40, 10, 9, 5, 4, 1};
    static const seq_t dots[]   = {     3,    5,    3,    5,    2,    6,    3, 7, 4, 6, 4, 6, 2};

    const size_t len = sizeof(values)/sizeof(values[0]);
    for (size_t i = 0; i < len; ++i)
    {
        idx_t count = n / values[i];
        if (count)
        {
            result += (seq_t)count * dots[i];
            n -= count * values[i];
        }
    }
    return result;
}
