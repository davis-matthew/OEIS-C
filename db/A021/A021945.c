/*
 * A021945  Decimal expansion of 1/941.
 * https://oeis.org/A021945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/941) mod 10, i.e. the decimal * digit at
 *            position n of the expansion of 1/941
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
    /* Static state to memoise previously computed digits. */
    static uint8_t *digits = NULL;        /* stored digits 0–9 */
    static size_t  cap     = 0;           /* allocated capacity */
    static size_t  len     = 0;           /* number of digits computed */
    static unsigned int rem   = 1u;        /* current remainder (0..940) */

    /* Grow the buffer if necessary. */
    if (len <= n && (len == cap || cap == 0)) {
        size_t new_cap = (cap == 0) ? 64 : cap * 2;
        if (new_cap <= len) new_cap = len + 1;

        uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
        if (!tmp) abort();          /* out of memory */
        digits = tmp;
        cap = new_cap;
    }

    /* Compute additional digits up to index n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        unsigned int q = (rem * 10u) / 941u;     /* digit */
        rem          = (rem * 10u) % 941u;        /* new remainder */
        digits[i]    = (uint8_t)q;
    }
    len = (size_t)n + 1;

    return (seq_t)digits[(size_t)n];
}
