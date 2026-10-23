/*
 * A021349  Decimal expansion of 1/345.
 * https://oeis.org/A021349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit of the expansion of 1/345
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;
    static idx_t cap = 0;
    static idx_t size = 0;
    static unsigned rem = 1;   /* remainder before generating the next digit */

    if (n < size)
        return digits[n];

    /* Ensure enough space for new digits */
    if (size == cap) {
        idx_t new_cap = cap ? cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed – shouldn't happen */
        digits = tmp;
        cap = new_cap;
    }

    /* Generate digits up to index n */
    while (size <= n) {
        rem *= 10;
        seq_t digit = (seq_t)(rem / 345u);
        rem %= 345u;
        digits[size] = digit;
        size++;

        if (size == cap) {
            idx_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            digits = tmp;
            cap = new_cap;
        }
    }

    return digits[n];
}
