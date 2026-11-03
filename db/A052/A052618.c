/*
 * A052618  Expansion of e.g.f. 1/((1-x)^2*(1-x^2)).
 * https://oeis.org/A052618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*floor((n+2)/2)*ceil((n+2)/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    static seq_t *fact = NULL;
    static size_t fact_size = 0;

    /* Extend factorial table up to n if necessary */
    if (n >= fact_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_fact = alloc_seq_array(new_size);

        /* Copy existing factorials */
        for (size_t i = 0; i < fact_size; ++i)
            new_fact[i] = fact[i];

        /* Initialise the factorial table */
        new_fact[0] = (seq_t)1;
        for (size_t i = 1; i <= (size_t)n; ++i)
            new_fact[i] = new_fact[i-1] * (seq_t)i;

        /* Free old table and store the new one */
        if (fact) free_seq_array(fact);
        fact = new_fact;
        fact_size = new_size;
    }

    /* Compute floor((n+2)/2) and ceil((n+2)/2) */
    idx_t m = n + 2;
    idx_t floor_half = m / 2;
    idx_t ceil_half  = m - floor_half;

    /* a(n) = n! * floor_half * ceil_half */
    seq_t result = fact[n];
    result = result * (seq_t)floor_half;
    result = result * (seq_t)ceil_half;
    return result;
}
