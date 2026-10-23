/*
 * A013322  Golomb-type sequence over triangular numbers.
 * https://oeis.org/A013322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013322: lengths of runs in a sequence of triangular numbers
 * where each k-th triangular number appears a(k) times.
 * This function constructs the sequence iteratively, keeping a cache
 * of already computed terms so that repeated calls are fast.
 * The recurrence is: let run r start with the r-th triangular number
 * T_r = r(r+1)/2.  The run is repeated exactly a(r) times,
 * where a(r) is the r-th term already generated.
 * The algorithm keeps the current run counter, the number of times
 * the current triangular number has been output and the current
 * triangular value.  When the required count is reached, the next
 * triangular number starts a new run.
 */
#include <stdlib.h>
#include <stddef.h>

static inline seq_t triangular(idx_t r) {
    /* compute r*(r+1)/2 using seq_t to avoid overflow */
    return ( (seq_t)r * ((seq_t)r + 1) ) / 2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1, but guard for n=0 */

    /* static cache for already computed sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;         /* number of elements stored */
    static size_t cache_cap = 0;         /* current capacity of cache */

    /* static state for the run construction algorithm */
    static idx_t cur_run_index = 0;      /* 0‑based index of current run */
    static idx_t count_in_run = 0;       /* how many times current run value
                                          has been output */
    static seq_t current_tri = 0;        /* value of current run (triangular) */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 1024;                /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cur_run_index = 0;
        count_in_run  = 0;
        current_tri = triangular(1);    /* first triangular number */
    }

    /* generate terms until we reach the requested index n */
    while (cache_len < (size_t)n) {
        /* ensure enough space */
        if (cache_len + 1 > cache_cap) {
            cache_cap <<= 1;            /* double capacity */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* output current value and update run counter */
        cache[cache_len] = current_tri;
        cache_len++;

        count_in_run++;

        /* check if current run has finished */
        if (count_in_run == cache[cur_run_index]) {
            /* move to next run */
            cur_run_index++;
            current_tri = triangular(cur_run_index + 1);
            count_in_run = 0;
        }
    }

    return cache[n - 1]; /* cache is 0‑based but n is 1‑based */
}
