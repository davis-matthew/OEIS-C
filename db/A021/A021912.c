/*
 * A021912  Decimal expansion of 1/908.
 * https://oeis.org/A021912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ((10^(n+1) / 908) % 10)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth digit after the decimal point in the expansion of 1/908.
   The result is stored and reused for future calls. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *digits = NULL;   /* cached digits */
    static size_t digits_len = 0;  /* number of digits currently cached */
    static uint32_t rem = 1;       /* remainder after the last digit */

    /* Grow the cache until it contains digit n */
    while (digits_len <= (size_t)n) {
        uint32_t t = rem * 10;                    /* multiply remainder by 10 */
        seq_t d = t / 908;                        /* next digit */
        rem = t % 908;                            /* new remainder */

        /* Reallocate the array to hold one more digit */
        seq_t *tmp = (seq_t *)realloc(digits,
                                      (digits_len + 1) * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort on error */
            abort();
        }
        digits = tmp;
        digits[digits_len++] = d;
    }
    return digits[(size_t)n];
}
