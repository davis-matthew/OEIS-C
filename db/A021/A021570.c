/*
 * A021570  Decimal expansion of 1/566.
 * https://oeis.org/A021570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ((10^(n+1) / 566) mod 10)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *digits = NULL;    /* stored digits */
    static idx_t cap     = 0;       /* capacity of digits array */
    static idx_t len     = 0;       /* number of digits stored */
    static idx_t rem     = 1;       /* remainder after last digit (valid when len>0) */

    /* If already computed, return */
    if (n < len)
        return digits[n];

    /* Allocate array on first use */
    if (cap == 0) {
        cap = 1024;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate digits up to index n */
    while (len <= n) {
        if (len >= cap) {
            cap *= 2;
            digits = (seq_t *)realloc(digits, cap * sizeof(seq_t));
        }
        rem = rem * 10;                     /* shift remainder */
        seq_t d = (seq_t)(rem / 566);       /* next decimal digit */
        rem = rem % 566;                    /* new remainder */
        digits[len++] = d;                  /* store digit */
    }

    return digits[n];
}
