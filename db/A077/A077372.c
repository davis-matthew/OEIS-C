/*
 * A077372  Fibonacci numbers whose external digits form a Fibonacci number.
 *          Or Fibonacci numbers whose MSD and LSD form a Fibonacci number.
 * https://oeis.org/A077372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 14, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate Fibonacci numbers, keeping those whose first
 * and last decimal digits together form a Fibonacci number.
 * The sequence a(n) is cached so that calls in any order run in
 * linear time with respect to the largest requested index. */

#include <stdbool.h>
#include <stdlib.h>

/* Return true if the two‑digit number formed by the most‑significant
 * and least‑significant decimal digits of x is itself a Fibonacci
 * number (0,1,2,3,5,8,13,21,34,55,89).  All one‑digit Fibonacci numbers
 * (including 0) are considered qualifying. */
static bool qualifies(seq_t x) {
    if (x < (seq_t)10) /* 0,1,1,2,3,5,8 automatically qualify */
        return true;

    seq_t last = x % (seq_t)10;

    seq_t first = x;
    while (first >= (seq_t)10) {
        first /= (seq_t)10;
    }

    int outer = (int)(first * 10 + last);  /* 10 … 99 */
    switch (outer) {
        case 0: case 1: case 2: case 3: case 5: case 8:
        case 13: case 21: case 34: case 55: case 89:
            return true;
        default:
            return false;
    }
}

seq_t a(idx_t n) {
    /* Static cache of qualifying Fibonacci numbers. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* State of the Fibonacci generator. */
    static seq_t fib_prev = (seq_t)0;  /* F(k) */
    static seq_t fib_curr = (seq_t)1;  /* F(k+1) */
    static bool first_call = true;

    if (first_call) {
        /* initialise on first use */
        cache = NULL;
        cache_cap = 0;
        cache_len = 0;
        fib_prev = (seq_t)0;
        fib_curr = (seq_t)1;
        first_call = false;
    }

    /* Generate Fibonacci numbers until we have at least n entries. */
    while (cache_len < (size_t)n) {
        if (qualifies(fib_prev)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure – undefined behaviour in this context */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = fib_prev;
        }

        /* Compute next Fibonacci number with overflow detection. */
        seq_t next = fib_prev + fib_curr;
        if (next < fib_prev) break;  /* overflow – cannot continue correctly */
        fib_prev = fib_curr;
        fib_curr = next;
    }

    return cache_len >= (size_t)n ? cache[n - 1] : (seq_t)0;
}
