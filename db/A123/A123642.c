/*
 * A123642  a(n) = n! - 2^n.
 * https://oeis.org/A123642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! - 2^n
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static caches to avoid recomputing for repeated calls */
    static seq_t *fact = NULL;
    static seq_t *pow2 = NULL;
    static size_t capacity = 0;   /* total allocated slots */
    static size_t last = 0;       /* last computed index (>=0) */

    /* Initialise caches on first call */
    if (capacity == 0) {
        capacity = 1;
        fact = (seq_t*)malloc(sizeof(seq_t) * capacity);
        pow2 = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (fact == NULL || pow2 == NULL) {
            /* Allocation failed – give a sensible default */
            return (seq_t)0;
        }
        fact[0] = (seq_t)1;   /* 0! = 1 */
        pow2[0] = (seq_t)1;   /* 2^0 = 1 */
        last = 0;
    }

    /* Grow cache if needed */
    while (n > last) {              /* compute up to n inclusive */
        /* Ensure enough space */
        if (last + 1 >= capacity) {
            size_t new_cap = capacity * 2;
            seq_t *new_fact = (seq_t*)realloc(fact, sizeof(seq_t) * new_cap);
            seq_t *new_pow2 = (seq_t*)realloc(pow2, sizeof(seq_t) * new_cap);
            if (new_fact == NULL || new_pow2 == NULL) {
                /* Allocation failed – fall back */
                return (seq_t)0;
            }
            fact = new_fact;
            pow2 = new_pow2;
            capacity = new_cap;
        }
        /* Compute next factorial and power of two */
        fact[last + 1] = fact[last] * (seq_t)(last + 1);
        pow2[last + 1] = pow2[last] * (seq_t)2;
        last++;
    }

    /* Return difference */
    return fact[n] - pow2[n];
}
