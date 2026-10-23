/*
 * A021557  Decimal expansion of 1/553.
 * https://oeis.org/A021557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ((10^(n+1) / 553) mod 10)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th digit after the decimal point in the expansion of 1/553 (0‑indexed). */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    #define PERIOD 78
    static int initialized = 0;
    static seq_t period[PERIOD];

    if (!initialized) {
        idx_t rem = 1;                     /* initial remainder after decimal point */
        for (idx_t i = 0; i < PERIOD; ++i) {
            rem *= 10;                     /* shift decimal */
            period[i] = (seq_t)(rem / 553);/* digit */
            rem %= 553;                    /* new remainder */
        }
        initialized = 1;
    }

    size_t idx = (size_t)(n % PERIOD);
    return period[idx];
}
