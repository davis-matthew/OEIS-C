/*
 * A021956  Decimal expansion of 1/952.
 * https://oeis.org/A021956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of the decimal expansion of 1/952 at index n
 *            (0‑indexed, a(0) is the first digit after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


seq_t a(idx_t n)
{
    static seq_t cycle[48];
    static bool initialized = false;

    if (!initialized) {
        idx_t rem = 1;                 /* starting remainder */

        /* Skip the non‑repeating part (first 3 digits). */
        for (idx_t i = 0; i < 3; ++i) {
            rem = (rem * 10) % 952;   /* remainder after digit i */
        }

        /* Generate the 48‑digit repeating cycle. */
        for (idx_t i = 0; i < 48; ++i) {
            rem = rem * 10;
            cycle[i] = (seq_t)(rem / 952);
            rem = rem % 952;
        }

        initialized = true;
    }

    if (n < 3) {
        switch ((int)n) {
            case 0: return 0;
            case 1: return 0;
            case 2: return 1;
        }
    }

    return cycle[(n - 3) % 48];
}
