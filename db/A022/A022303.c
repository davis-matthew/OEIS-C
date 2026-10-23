/*
 * A022303  The sequence a of 1's and 2's starting with (1,2,1) such that a(n)
 *          is the length of the (n+2)nd run of a.
 * https://oeis.org/A022303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9942, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=1; for n≥1, a(n) is the length of the run (n+2)-th of the
   sequence whose values alternate 1,2,1,2,…  (Kolakoski-variant). */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t run_idx = 0;            /* index (1‑based) of the run that will be generated next */
static idx_t run_len_remaining = 0;  /* number of terms left in the current run */
static seq_t run_value = 0;          /* value of the current run (1 or 2) */

static void grow_cache(idx_t new_cap)
{
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(idx_t need)
{
    if (cache != NULL)
        return;

    /* initial capacity (will grow as needed) */
    cache_cap = 64;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));

    /* seed first three terms of a */
    cache[0] = (seq_t)1; /* a(1) */
    cache[1] = (seq_t)2; /* a(2) */
    cache[2] = (seq_t)1; /* a(3) */
    cache_len = 3;

    /* prepare the first run that will be produced after the seed */
    run_idx = 4; /* next run number (1‑based) */
    run_value = ((run_idx & 1) ? (seq_t)1 : (seq_t)2); /* alternating 1,2,1,… */
    /* length of run 4 equals a(2) */
    run_len_remaining = (idx_t)cache[run_idx - 3];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 1; safety guard */

    init_cache(n);

    /* generate terms until we have at least n entries */
    while (cache_len < n) {
        if (cache_len == cache_cap)
            grow_cache(cache_cap * 2);

        /* emit next term of the current run */
        cache[cache_len++] = run_value;

        /* decrement terms left in this run */
        if (--run_len_remaining == 0) {
            /* start next run */
            ++run_idx;
            run_value = ((run_idx & 1) ? (seq_t)1 : (seq_t)2);

            if (run_idx <= 3) {          /* runs 1,2,3 are the fixed seed runs */
                run_len_remaining = 1;
            } else {
                /* length = a(run_idx-2) = cache[run_idx-3] */
                run_len_remaining = (idx_t)cache[run_idx - 3];
            }
        }
    }

    return cache[n - 1];
}
