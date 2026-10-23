/*
 * A021427  Decimal expansion of 1/423.
 * https://oeis.org/A021427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/423) mod 10 (decimal digits of 1/423;
 *            period 46)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* period of 1/423 is 46, so a(n) = a(n % 46) */
    static seq_t digits[46];
    static bool initialized = false;

    if (!initialized)
    {
        uint64_t r = 1;           /* current remainder */
        for (idx_t i = 0; i < 46; ++i)
        {
            r = r * 10ULL;       /* multiply by 10 */
            digits[i] = (seq_t)(r / 423ULL);  /* digit */
            r = r % 423ULL;      /* new remainder */
        }
        initialized = true;
    }

    return digits[n % 46];
}
