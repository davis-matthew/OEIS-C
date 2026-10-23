/*
 * A021298  Decimal expansion of 1/294.
 * https://oeis.org/A021298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The decimal expansion of 1/294.  a(n) is the n‑th digit after the decimal point. */
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t cycle[42];
    static bool init = false;

    if (!init) {
        idx_t rem = 1;
        /* Skip the two initial non‑repeating zeros (both digits are 0). */
        for (idx_t i = 0; i < 2; ++i) {
            rem *= 10;
            rem %= 294;
        }

        /* Generate the 42 digits of the repeating part. */
        for (idx_t i = 0; i < 42; ++i) {
            rem *= 10;
            cycle[i] = rem / 294;
            rem %= 294;
        }
        init = true;
    }

    if (n < 2)
        return (seq_t)0;

    return cycle[(n - 2) % 42];
}
