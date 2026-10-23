/*
 * A115096  Decimal expansion of ratio 7129 / 6105195.
 * https://oeis.org/A115096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) is the nth decimal digit after the point in 7129/6105195,
 *            computed by long division with memoisation
 * Range:     u8 none, u16 none, u32 n <= 104, u64 n <= 104, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* for uint8_t */

seq_t a(idx_t n)
{
    /* Constants of the fraction */
    static const seq_t P = 7129;          /* numerator   */
    static const seq_t Q = 6105195;       /* denominator */

    /* State for incremental computation */
    static seq_t rem = P;                /* current remainder, starts with the numerator    */
    static idx_t  len = 0;               /* number of digits already computed              */
    static idx_t  cap = 0;               /* allocated capacity of the digit buffer          */
    static uint8_t *digits = NULL;       /* array holding digits of the decimal expansion */

    /* Compute digits up to position n if necessary */
    if (len <= n) {
        /* initial allocation */
        if (cap == 0) {
            cap = 16;
            digits = (uint8_t *)malloc(cap * sizeof(uint8_t));
        }

        /* long‑division loop */
        while (len <= n) {
            seq_t digit = (rem * (seq_t)10) / Q;   /* current decimal digit   */
            rem = (rem * (seq_t)10) % Q;           /* new remainder           */

            /* grow array when needed */
            if (len >= cap) {
                cap <<= 1;  /* double the capacity */
                digits = (uint8_t *)realloc(digits, cap * sizeof(uint8_t));
            }
            digits[len++] = (uint8_t)digit;
        }
    }

    return (seq_t)digits[n];
}
