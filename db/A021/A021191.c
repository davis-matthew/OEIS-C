/*
 * A021191  Decimal expansion of 1/187.
 * https://oeis.org/A021191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Periodic digit sequence of 1/187: 0,0,5,3,4,7,5,9,3,5,8,2,8,8,7,7 (period 16). */
seq_t a(idx_t n)
{
    static seq_t period[16];
    static bool init = false;
    if (!init) {
        idx_t rem = 1;   /* starting remainder of the long division */
        for (idx_t i = 0; i < 16; ++i) {
            rem *= 10;                     /* bring down next digit */
            period[i] = rem / 187;         /* the quotient is the digit */
            rem %= 187;                    /* new remainder */
        }
        init = true;
    }
    return period[n & 15];   /* index modulo 16 (period length) */
}
