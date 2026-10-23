/*
 * A021820  Decimal expansion of 1/816.
 * https://oeis.org/A021820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   decimal expansion of 1/816. The first 4 digits are
 *            non‑repeating: 0,0,1,2. Then a period of 16 digits repeats:
 *            2,5,4,9,0,1,9,6,0,7,8,4,3,1,3,7
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t pre[4] = {0, 0, 1, 2};
    static seq_t period[16];
    static bool initialized = false;

    if (!initialized) {
        /* compute the first 20 digits (4 preperiod + 16 period) */
        uint64_t rem = 1;               /* remainder of 1/816 */
        seq_t  digits[20];
        for (idx_t i = 0; i < 20; ++i) {
            rem *= 10;
            digits[i] = (seq_t)(rem / 816);
            rem      = rem % 816;
        }
        /* extract the period starting at index 4 */
        for (idx_t i = 0; i < 16; ++i)
            period[i] = digits[i + 4];
        initialized = true;
    }

    if (n < 4)
        return pre[n];
    else
        return period[(n - 4) % 16];
}
