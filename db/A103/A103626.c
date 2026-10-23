/*
 * A103626  Expansion of (1 + x + x^2 + x^3 + x^5 + x^6 + x^7 - x^8 + x^10 +
 *          x^11 - 2*x^12 - x^13 - x^14)/(1 - x^4 - x^8 - x^12 + 2*x^16).
 * https://oeis.org/A103626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 101, u32 n <= 206, u64 n <= 418,
 *            u128 n <= 841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-4) + a(n-8) + a(n-12) - 2*a(n-16);
   Base values for n = 0..15 are
   1,1,1,1,1,2,2,2,1,3,4,4,1,5,6,7   (the first 16 terms of A103626) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table of computed values */
    static seq_t *values = NULL;
    static idx_t max_n   = 0;   /* highest n that has been computed */
    static idx_t capacity = 0;  /* allocated size of the table */

    /* initial values for n = 0..15 */
    static const seq_t init_vals[16] =
    { 1,1,1,1,1,2,2,2,1,3,4,4,1,5,6,7 };

    /* first call – initialise the table with the first 16 terms */
    if (values == NULL) {
        capacity = 16;
        values = (seq_t*)malloc(capacity * sizeof(seq_t));
        for (idx_t i = 0; i < capacity; ++i)
            values[i] = init_vals[i];
        max_n = 15;
    }

    /* if n is already known simply return it */
    if (n <= max_n)
        return values[n];

    /* enlarge the table if necessary */
    if (n + 1 > capacity) {
        capacity = n + 1;
        values = (seq_t*)realloc(values, capacity * sizeof(seq_t));
    }

    /* compute the new values using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        values[i] = values[i-4] + values[i-8] + values[i-12]
                    - (seq_t)2 * values[i-16];
    }
    max_n = n;
    return values[n];
}
