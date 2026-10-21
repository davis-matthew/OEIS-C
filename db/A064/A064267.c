/*
 * A064267  Clavis terminorum in Julian calendar for a year with Golden Number
 *          n.
 * https://oeis.org/A064267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Curve: a(n) = table[(n-1) % 19]
   The sequence A064267 repeats every 19 terms. This implementation
   stores the 19 base values and maps any n ≥ 1 to the corresponding
   term via modulo arithmetic. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base 19 terms for n = 1…19 (periodic extension for all n ≥ 1). */
    static const seq_t table[19] = {
        26, 15, 34, 23, 12,
        31, 20, 39, 28, 17,
        36, 25, 14, 33, 22,
        11, 30, 19, 38
    };

    /* Convert the one‑based index n to a zero‑based index in [0,18]. */
    idx_t idx = (n - 1) % 19;

    return table[(size_t)idx];
}
