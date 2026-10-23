/*
 * A021116  Decimal expansion of 1/112.
 * https://oeis.org/A021116
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

/* Sequence A021116: decimal expansion of 1/112.
   a(n) is the nth digit (n starts at 0) of 1/112.  
   Computed by successive long‑division:   (rem*10)/112 gives the next digit,
   and rem is updated to (rem*10)%112. */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;        /* current remainder (<112) */
    seq_t digit = 0;         /* digit to return */

    /* Perform long division up to the nth digit. */
    for (idx_t i = 0; i <= n; ++i) {
        digit = (seq_t)((rem * 10ULL) / 112ULL);
        rem   = (rem * 10ULL) % 112ULL;
    }

    return digit;
}
