/*
 * A021267  Decimal expansion of 1/263.
 * https://oeis.org/A021267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10ⁿ / 263⌋ mod 10 – the n‑th digit of 1/263
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
    /* Static state for incremental computation and caching   */
    static uint8_t *digits  = NULL;   /* array of computed digits (0–9) */
    static size_t   cap     = 0;      /* capacity of 'digits' array   */
    static idx_t   count   = 0;       /* number of digits stored       */
    static uint64_t rem    = 1;       /* remainder after the last digit */

    if (n >= count) {
        /* Grow the array to hold at least n+1 digits */
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > cap) {
            uint8_t *tmp = (uint8_t *)realloc(digits, new_cap);
            if (!tmp) return 0;               /* allocation failure */
            digits = tmp;
            cap    = new_cap;
        }

        /* Compute digits from count to n inclusive */
        for (idx_t i = count; i <= n; ++i) {
            uint64_t ten = rem * 10ULL;       /* current remainder * 10   */
            uint8_t d    = (uint8_t)(ten / 263ULL);   /* next digit          */
            rem         = ten % 263ULL;       /* update remainder        */
            digits[i]   = d;                  /* store the digit         */
        }
        count = n + 1;                       /* update number of digits */
    }

    return (seq_t)digits[n];                 /* return requested digit  */
}
