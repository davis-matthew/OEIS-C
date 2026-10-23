/*
 * A021543  Decimal expansion of 1/539.
 * https://oeis.org/A021543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes nth decimal digit of 1/539 via long division. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed digits. */
    static seq_t *digits = NULL;
    static size_t capacity = 0;      /* size of allocated array */
    static size_t next_index = 0;    /* next index to compute */
    static uint64_t rem = 1;         /* remainder, fits in uint64_t */

    /* Grow cache as needed. */
    if (n >= capacity) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return 0 as safe fallback */
            return (seq_t)0;
        }
        digits = tmp;
        capacity = new_cap;
    }

    /* Compute missing digits from next_index up to n. */
    for (size_t i = next_index; i <= (size_t)n; ++i) {
        rem *= 10ULL;                         /* safe intermediate type    */
        uint64_t d = rem / 539ULL;            /* digit (0-9)                */
        rem %= 539ULL;                        /* new remainder              */
        digits[i] = (seq_t)d;                 /* store as seq_t             */
    }
    next_index = (size_t)n + 1;

    return digits[(size_t)n];
}
