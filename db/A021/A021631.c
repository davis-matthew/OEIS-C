/*
 * A021631  Decimal expansion of 1/627.
 * https://oeis.org/A021631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th digit after the decimal point in the expansion
 *            of 1/627
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period length is 18 digits. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The decimal expansion of 1/627 is periodic with period 18.
       Compute the period once and reuse it. */
    static seq_t period[18];
    static bool initialized = false;

    if (!initialized) {
        idx_t r = 1U;          /* current remainder (< 627)          */
        for (idx_t i = 0; i < 18; ++i) {
            r *= 10U;               /* multiply remainder by 10          */
            period[i] = (seq_t)(r / 627U); /* next digit                 */
            r %= 627U;              /* new remainder                     */
        }
        initialized = true;
    }

    return period[n % 18U];
}
