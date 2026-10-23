/*
 * A021667  Decimal expansion of 1/663.
 * https://oeis.org/A021667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/663 (period 48)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t digits[48];
    static bool initialized = false;

    if (!initialized)
    {
        /* Compute the 48‑digit repeating block of 1/663 */
        seq_t r = 1;              /* starting remainder */
        for (size_t i = 0; i < 48; ++i)
        {
            seq_t d = (10 * r) / 663;        /* next digit */
            digits[i] = d;
            r = (10 * r) % 663;              /* new remainder */
        }
        initialized = true;
    }
    return digits[(size_t)(n % 48)];
}
