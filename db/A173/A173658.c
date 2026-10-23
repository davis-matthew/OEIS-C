/*
 * A173658  Similar to A093506, but starting with 2, 4.
 * https://oeis.org/A173658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173658
   a(n) is the n‑th element of a permutation of the natural numbers
   obtained by concatenating runs of constant parity.
   Run 1 (odd index) uses the smallest unused even numbers and has
   length equal to a(1)=2.
   Run 2 (even index) uses the smallest unused odd numbers and has
   length a(2)=4.
   For r>2 the length of run r is the r‑th term of the sequence,
   i.e. a(r).  The sequence is a permutation of the naturals and
   has offset 1.  The implementation generates the required runs
   on demand and caches all printed terms so that subsequent calls
   are served in O(1) per term.  No external libraries are used. */

#include <stdlib.h>  /* malloc, realloc */
#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    if (n == 0u)                 /* offset 1; undefined – return 0 */
        return (seq_t)0;

    /* static state to support non‑monotonic calls */
    static idx_t     terms_done = 0u;    /* number of terms already cached   */
    static idx_t     next_run  = 1u;     /* next run number to generate       */
    static idx_t     next_even = 2u;     /* next unused even number           */
    static idx_t     next_odd  = 1u;     /* next unused odd  number           */
    static idx_t    *cache     = NULL;  /* 0‑based array of already produced terms  */
    static size_t    cache_cap = 0u;     /* current capacity of cache          */

    /* enlarge cache so that it can store up to 'n' terms */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16u;
        while (new_cap < n)
            new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        if (tmp == NULL) abort();      /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (terms_done < n) {
        /* length of the current run r */
        idx_t run_len = (next_run == 1u) ? 2u : cache[next_run - 1u];

        /* generate the run_len terms of this run */
        for (idx_t i = 0u; i < run_len; ++i) {
            idx_t val;
            if (next_run & 1u) {            /* odd run → even numbers */
                val = next_even;
                next_even += 2u;
            } else {                        /* even run → odd numbers */
                val = next_odd;
                next_odd += 2u;
            }

            /* ensure cache large enough for the new term */
            if (terms_done >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap : 16u;
                while (new_cap <= terms_done + 1u)
                    new_cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                if (tmp == NULL) abort();
                cache     = tmp;
                cache_cap = new_cap;
            }

            cache[terms_done++] = val;
        }
        ++next_run;   /* move to the next run */
    }

    /* return the requested term as seq_t */
    return (seq_t)cache[n - 1u];
}
