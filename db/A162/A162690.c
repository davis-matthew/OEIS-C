/*
 * A162690  Strictly positive numbers k such that 20*k/(20+k) are integers.
 * https://oeis.org/A162690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162690: strictly positive k such that 20*k/(20+k) is an integer.
   The values are obtained from the formula  k = 400/d - 20,
   where d runs through the divisors of 400 that are ≤ 19 in decreasing order:
   16, 10, 8, 5, 4, 2, 1.  */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* divisors of 400 ≤ 19 in the order that yields increasing k */
    static const idx_t divs[] = {16, 10, 8, 5, 4, 2, 1};
    const size_t len = sizeof(divs) / sizeof(divs[0]);

    if (n >= 1 && n <= len) {
        idx_t d = divs[n - 1];
        seq_t k = (seq_t)(400 / d - 20);
        return k;
    }
    return 0;
}
