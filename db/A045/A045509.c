/*
 * A045509  Factorials having initial digit '1'.
 * https://oeis.org/A045509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A045509: factorials whose first decimal digit is 1
   by incremental multiplication and integer digit extraction. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of results, growing lazily */
    static seq_t *cache = NULL;          /* array of factorials whose
                                          first digit is 1 */
    static size_t cache_cap = 0, cache_len = 0;

    /* State for generating factorials incrementally */
    static seq_t fact = 1;  /* (m-1)!  (0! on first call) */
    static idx_t  m    = 1; /* next multiplier to apply (starts at 1) */
    static seq_t pow10 = 1; /* 10^(d-1), where d = number of digits
                      of the current factorial, initially 1 (d=1) */

    /* Initialise static storage on first use */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();   /* allocation failed */
        cache_len = 0;
        fact = 1;      /* 0! = 1, will become 1! after first increment */
        m    = 1;
        pow10 = 1;
    }

    /* Generate factorials until we have enough terms for the requested index */
    while (cache_len < (size_t)n) {
        /* Compute next factorial: fact = fact * m   (now fact = m!) */
        fact = fact * (seq_t)m;

        /* Update pow10 to match the new number of decimal digits of fact.
           While fact >= pow10*10, we cross a power of ten boundary. */
        in_loop:
        if (fact >= pow10 * (seq_t)10) {
            pow10 *= (seq_t)10;
            goto in_loop;
        }

        /* Leading digit is fact / pow10 (integer division, truncating) */
        seq_t leading_digit = fact / pow10;
        if (leading_digit == 1) {
            /* Store the factorial in the cache, resizing if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();     /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = fact;
        }

        /* Prepare for next factorial */
        ++m;
    }

    /* Return the (n‑1)th element of the cache (0‑based index) */
    return cache[(size_t)(n - 1)];
}
