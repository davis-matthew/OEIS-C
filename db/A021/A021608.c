/*
 * A021608  Decimal expansion of 1/604.
 * https://oeis.org/A021608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/604 (0‑based, with a(0) = 0).
 *            The nth digit equals floor(10 * (10^n mod 604) / 604)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage of already computed digits */
    static seq_t *digits = NULL;
    static size_t  size   = 0;   /* how many digits are cached */
    static size_t  cap    = 0;   /* current capacity of digits array */
    static seq_t  remainder = 1; /* remainder before computing the next digit */

    /* if we have the digit already, just return it */
    if (n < size)
        return digits[(size_t)n];

    /* need to extend the cache up to index n inclusive */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        /* grow capacity: double until we are large enough */
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap < need)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp && new_cap)
            abort();      /* memory exhaustion – terminate */
        digits = tmp;
        cap    = new_cap;
    }

    /* compute missing digits */
    for (size_t i = size; i < need; ++i) {
        remainder *= (seq_t)10;
        seq_t digit = remainder / (seq_t)604;
        remainder %= (seq_t)604;
        digits[i] = digit;
    }

    size = need;
    return digits[(size_t)n];
}
