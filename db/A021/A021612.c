/*
 * A021612  Decimal expansion of 1/608.
 * https://oeis.org/A021612
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

/* Sequence: decimal expansion of 1/608
   a(n) is the nth digit after the decimal point of 1/608
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;          /* current remainder before generating a digit */
    seq_t digit = 0;           /* digit produced in the current step */

    /* Perform n+1 long‑division steps (steps are numbered 0,1,…,n)
       to obtain the digit at position n. */
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t tmp = rem * 10ULL;   /* shift remainder left by one decimal place */
        digit = (seq_t)(tmp / 608ULL);/* next digit */
        rem   = tmp % 608ULL;        /* new remainder after division */
    }

    return digit;
}
