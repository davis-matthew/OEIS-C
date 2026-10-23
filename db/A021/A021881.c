/*
 * A021881  Decimal expansion of 1/877.
 * https://oeis.org/A021881
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

/* A021881 – nth decimal digit of 1/877 in base 10. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of digits: enough for n up to at least 10000. */
    static uint8_t digits[20001];
    static idx_t len   = 0;   /* number of digits cached so far */
    static idx_t rem   = 1;   /* current remainder (start with 1/877) */

    /* Compute digits up to index n on demand. */
    while (len <= n) {
        idx_t tmp = rem * (idx_t)10;       /* bring down a zero */
        uint8_t d = (uint8_t)(tmp / (idx_t)877); /* next decimal digit */
        rem = tmp % (idx_t)877;            /* new remainder */
        digits[len++] = d;
    }
    return (seq_t)digits[n];
}
