/*
 * A021917  Decimal expansion of 1/913.
 * https://oeis.org/A021917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth decimal digit of 1/913 after the decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;
    static size_t capacity = 0;
    static size_t len = 0;          /* number of digits already computed */
    static unsigned long long rem = 1;  /* current remainder (starts with 1) */

    /* initialize storage */
    if (digits == NULL) {
        capacity = 256;
        digits = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (!digits) return (seq_t)0; /* fallback, should not happen */
    }

    /* grow storage if necessary */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(digits, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0; /* fallback, should not happen */
        digits = tmp;
        capacity = new_cap;
    }

    /* compute missing digits up to n */
    if ((size_t)n >= len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            rem = rem * 10ULL;
            unsigned digit = (unsigned)(rem / 913ULL);
            rem = rem % 913ULL;
            digits[i] = (seq_t)digit;
        }
        len = (size_t)n + 1;
    }

    return digits[(size_t)n];
}
