/*
 * A021976  Decimal expansion of 1/972.
 * https://oeis.org/A021976
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

/* Decimal expansion of 1/972.  For n > 28, a(n) = a(n-27).  The first 29
   digits a(0)…a(28) are computed once by long division. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t prec[29];
    static bool init = false;

    if (!init) {
        uint64_t rem = 1;
        for (idx_t i = 0; i < 29; ++i) {
            rem *= 10ULL;
            prec[i] = (seq_t)(rem / 972ULL);
            rem    = rem % 972ULL;
        }
        init = true;
    }

    idx_t m = n;
    while (m > 28) {
        m -= 27;
    }
    return prec[m];
}
