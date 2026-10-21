/*
 * A084915  a(n) = (n!)^2*n.
 * https://oeis.org/A084915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * n! * n (equivalently, n!*(n+1)! - n!^2, offset 0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static tables of factorials and of a(n) values. */
    static seq_t *fact = NULL;       /* fact[i] = i!          */
    static seq_t *value = NULL;      /* value[i] = a(i)       */
    static idx_t max_n = 0;          /* largest index computed */
    static idx_t capacity = 0;       /* allocated array size   */

    /* First call – initialise the tables for index 0. */
    if (!fact) {
        capacity = 1;
        fact = (seq_t *)malloc(sizeof(seq_t) * capacity);
        value = (seq_t *)malloc(sizeof(seq_t) * capacity);
        fact[0] = (seq_t)1;         /* 0! = 1 */
        value[0] = (seq_t)0;        /* a(0) = 0 */
        max_n = 0;
    }

    /* Ensure the tables are large enough to hold index n. */
    if (n > max_n) {
        idx_t required = n + 1;

        /* Expand the allocated space if necessary. */
        if (required > capacity) {
            capacity = required * 2;
            fact = (seq_t *)realloc(fact, sizeof(seq_t) * capacity);
            value = (seq_t *)realloc(value, sizeof(seq_t) * capacity);
        }

        /* Compute factorials and a(i) for all new indices. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* i! = (i-1)! * i */
            fact[i] = fact[i - 1] * (seq_t)i;

            /* a(i) = fact[i] * fact[i] * i */
            seq_t temp = fact[i];
            temp = temp * fact[i];
            temp = temp * (seq_t)i;
            value[i] = temp;
        }
        max_n = n;
    }

    return value[n];
}
