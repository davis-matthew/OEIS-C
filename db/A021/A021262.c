/*
 * A021262  Decimal expansion of 1/258.
 * https://oeis.org/A021262
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

/* Decimal expansion of 1/258.
   The first two digits are 0,0 (pre‑period), followed by a
   period of 21 digits: 3,8,7,5,9,6,8,9,9,2,2,4,8,0,6,2,0,1,5,5,0.
   Thus for n>=2: a(n) = cycle[(n-2) % 21] . */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return pre‑period digit */
    if (n < 2)
        return 0U;

    /* Periodic part */
    static const seq_t cycle[21] = {
        3U, 8U, 7U, 5U, 9U,
        6U, 8U, 9U, 9U, 2U,
        2U, 4U, 8U, 0U, 6U,
        2U, 0U, 1U, 5U, 5U, 0U
    };
    idx_t idx = (n - 2) % 21;
    return cycle[idx];
}
