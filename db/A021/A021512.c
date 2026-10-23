/*
 * A021512  Decimal expansion of 1/508.
 * https://oeis.org/A021512
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

/* Decimal expansion of 1/508.  The n‑th term is the n‑th digit of the
 * decimal representation of 1/508, starting with a(0) = 0.  The
 * computation uses long division and memoises the digits already
 * produced. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cached digits (0‑9).  We store them in an array of uint8_t to
     * avoid reallocating an array of the arbitrary‑precision class
     * type.  The return value is converted to seq_t when needed. */
    static uint8_t *digits = NULL;
    static size_t cap = 0;     /* allocated capacity of digits[] */
    static size_t used = 0;    /* number of digits actually stored */
    static uint64_t remainder = 1;  /* current remainder of div(10^k, 508) */

    /* Ensure we have computed digits up to index n. */
    while (used <= (size_t)n) {
        /* Grow the cache if necessary. */
        if (used == cap) {
            size_t new_cap = (cap == 0) ? 1 : (cap << 1);
            while (new_cap <= (size_t)n)
                new_cap <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(digits, new_cap);
            if (!tmp) abort();   /* allocation failed */
            digits = tmp;
            cap = new_cap;
        }

        /* Compute the next digit by long division: digit = floor(rem*10/508) */
        uint16_t digit = (uint16_t)((remainder * 10ULL) / 508ULL);
        remainder = (remainder * 10ULL) % 508ULL;
        digits[used++] = (uint8_t)digit;
    }

    /* Return the requested digit, converted to seq_t. */
    return (seq_t)digits[(size_t)n];
}
