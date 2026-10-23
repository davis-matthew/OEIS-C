/*
 * A014728  Squares of odd Fibonacci numbers.
 * https://oeis.org/A014728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014437(n)^2 (squares of odd Fibonacci numbers)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute a(n).  Uses a simple static cache that grows as needed. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;        /* cache of a(n) values */
    static size_t cache_cap = 0;       /* allocated capacity of cache   */
    static size_t odd_cnt   = 0;       /* number of odd Fibonacci values cached */
    static seq_t f_prev = 0, f_curr = 0;  /* F_{k-1} and F_k */
    static idx_t  idx = 0;             /* current Fibonacci index (k)   */

    /* Initialise on first call */
    if (cache == NULL) {
        cache_cap = 4;                  /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;                    /* out of memory – return 0 as sentinel */

        /* first two odd Fibonacci numbers: F1 and F2 */
        cache[0] = (seq_t)1;            /* a(0) */
        cache[1] = (seq_t)1;            /* a(1) */
        odd_cnt = 2;

        /* start Fibonacci generation at index 2 (F2) */
        f_prev = (seq_t)1;              /* F1 */
        f_curr = (seq_t)1;              /* F2 */
        idx     = 2;                    /* we have already processed F2 */
    }

    /* Generate more odd Fibonacci squares until we have one for index n */
    while (odd_cnt <= (size_t)n) {
        /* Compute next Fibonacci number */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        ++idx;                          /* advance to next index */

        /* If the current Fibonacci number is odd (index mod 3 != 0) */
        if (idx % 3 != 0) {
            /* Square the odd Fibonacci number and store it */
            seq_t square = f_curr * f_curr;

            /* Grow cache if necessary */
            if (odd_cnt >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL)
                    return 0;            /* out of memory – return 0 as sentinel */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[odd_cnt] = square;
            ++odd_cnt;
        }
    }

    return cache[(size_t)n];
}
