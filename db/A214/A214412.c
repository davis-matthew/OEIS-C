/*
 * A214412  Numbers that can't be expressed as the sum of a Fibonacci number
 *          and a square of a positive integer.
 * https://oeis.org/A214412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers that cannot be expressed as the sum of a
 *            Fibonacci number (0 is allowed) and a square of a positive
 *            integer
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Helper to grow a seq_t array and keep track of its capacity. */
static void grow_seq_array_always(seq_t **arr, size_t *cap, size_t new_len) {
    /* Use the existing util if available, otherwise a simple realloc. */
    *cap = (new_len > *cap) ? (new_len * 2) : *cap;
    if (*cap == 0) *cap = 1;
    *arr = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
}

/* Generates the sequence a(n). */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap = 0;    /* capacity of cache array */

    static seq_t *fibs = NULL;      /* list of Fibonacci numbers */
    static size_t fibs_size = 0;
    static size_t fibs_cap = 0;

    static idx_t next_candidate = 0;  /* next integer to test for representability */

    /* Ensure at least two Fibonacci numbers are available */
    if (fibs_size < 2) {
        grow_seq_array_always(&fibs, &fibs_cap, 2);
        fibs[0] = (seq_t)0;
        fibs[1] = (seq_t)1;
        fibs_size = 2;
    }

    /* Compute more terms until we have at least n of them. */
    while (cache_size < n) {
        seq_t x = (seq_t)next_candidate;
        next_candidate++;

        /* Expand Fibonacci list up to x */
        while (fibs_size > 0 && fibs[fibs_size - 1] <= x) {
            /* compute next Fibonacci number */
            seq_t next_fib;
            if (fibs_size == 1)
                next_fib = fibs[0] + fibs[0];   /* 0+0 -> 0 (not used) */
            else
                next_fib = fibs[fibs_size - 1] + fibs[fibs_size - 2];

            grow_seq_array_always(&fibs, &fibs_cap, fibs_size + 1);
            fibs[fibs_size++] = next_fib;
        }

        /* Check if x can be written as Fibonacci + square */
        bool representable = false;
        for (size_t i = 0; i < fibs_size; ++i) {
            seq_t f = fibs[i];
            if (f > x) break;
            seq_t d = x - f;
            if (d < (seq_t)1) continue;        /* square must be positive */

            seq_t s = floor_sqrt(d);
            if (s * s == d) {
                representable = true;
                break;
            }
        }

        if (!representable) {
            grow_seq_array_always(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = x;
        }
    }

    return cache[n - 1];  /* a(1) is the first term in the cache */
}
