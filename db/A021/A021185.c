/*
 * A021185  Decimal expansion of 1/181.
 * https://oeis.org/A021185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   decimal digit of 1/181 at position n (0‑indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state to store computed digits */
    static seq_t *digits = NULL;   /* array of computed digits */
    static size_t len    = 0;      /* number of digits stored */
    static size_t cap    = 0;      /* current allocation capacity */
    static idx_t  rem    = 1;      /* current remainder (initially 1 for 1/181) */

    /* Return cached digit if already computed */
    if (n < len)
        return digits[n];

    /* Allocate or grow the digit array if necessary */
    if (digits == NULL) {
        cap = 64;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate digits up to index n */
    for (; len <= n; ++len) {
        if (len >= cap) {
            cap <<= 1;                    /* double the capacity */
            digits = (seq_t *)realloc(digits, cap * sizeof(seq_t));
        }
        idx_t prod  = rem * 10;           /* multiply remainder by 10 */
        seq_t digit = (seq_t)(prod / 181);/* digit at this position   */
        digits[len] = digit;              /* store the digit        */
        rem         = prod % 181;         /* update remainder       */
    }

    return digits[n];
}
