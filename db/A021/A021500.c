/*
 * A021500  Decimal expansion of 1/496.
 * https://oeis.org/A021500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-15) for n > 18 Initial values a(0)..a(18) are the
 *            first 19 digits of the decimal expansion of 1/496, computed by
 *            long division
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The first 19 digits of 1/496.  They are computed
       once (lazy initialization) by long division. */
    enum { INIT_LEN = 19 };          /* 0 .. 18 inclusive */
    static seq_t init[INIT_LEN];
    static bool initialized = false;

    if (!initialized) {
        const idx_t DEN = 496;       /* divisor 1/496 */
        idx_t rem = 1;              /* current remainder */
        for (idx_t i = 0; i < INIT_LEN; ++i) {
            idx_t digit = (rem * 10ULL) / DEN;
            rem = (rem * 10ULL) % DEN;
            init[i] = (seq_t)digit;
        }
        initialized = true;
    }

    /* From a(n) = a(n-15) for n > 18 we can reduce the index
       until it lies in the range 0..18. */
    idx_t idx = n;
    while (idx > 18) {
        idx -= 15;
    }
    return init[idx];
}
