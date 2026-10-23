/*
 * A021540  Decimal expansion of 1/536.
 * https://oeis.org/A021540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 536) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the (n)th decimal digit of 1/536, with offset 0. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t *digits = NULL;
    static size_t capacity = 0;
    static idx_t computed = 0;            /* number of digits already computed */
    static uint64_t rem = 1;              /* remainder after last step (start with 1) */

    /* ensure at least one byte of memory is allocated initially */
    if (digits == NULL) {
        capacity = 1;
        digits = (seq_t *)malloc(capacity * sizeof(seq_t));
        /* pthread-safe initialization is not required for build environment */
    }

    /* extend cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t newcap = capacity;
        while (newcap <= (size_t)n) {
            newcap = newcap * 2;
            if (newcap == 0) newcap = 16; /* guard against overflow */
        }
        seq_t *tmp = (seq_t *)realloc(digits, newcap * sizeof(seq_t));
        if (tmp != NULL) {
            digits = tmp;
            capacity = newcap;
        }
        /* In a production environment we would handle realloc failure here. */
    }

    /* compute missing digits up to n */
    while (computed <= n) {
        /* long division step: compute next decimal digit */
        uint64_t prod = rem * 10ULL;
        uint64_t digit = prod / 536ULL;      /* digit 0-9 */
        rem = prod % 536ULL;
        digits[computed] = (seq_t)digit;
        computed++;
    }

    return digits[n];
}
