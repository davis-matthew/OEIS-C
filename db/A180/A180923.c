/*
 * A180923  Numbers n such that 111*n + 1 is prime.
 * https://oeis.org/A180923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180923: Numbers n such that 111*n + 1 is prime. */
#include <stdint.h>
#include <stdlib.h>

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

/* Indexed from 1; returns the n-th positive integer m such that 111*m+1 is prime. */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of cached terms */
    static size_t capacity   = 0;      /* current allocation size of cache */
    static idx_t last_checked = 1;     /* next integer to test for primality */

    /* Extend cache until we have at least n terms */
    while (cache_size < n)
    {
        /* test successive integers until a new term is found */
        for (idx_t m = last_checked; ; ++m)
        {
            seq_t value = ((seq_t)111) * (seq_t)m + 1;
            if (is_prime_seq(value))
            {
                /* grow cache if necessary */
                if (cache_size + 1 > capacity)
                    ensure_capacity(cache_size + 1, &capacity, (void **)&cache, sizeof(seq_t));

                cache[cache_size++] = m;  /* store the found term */
                last_checked = m + 1;     /* next search starts after this m */

                /* if we reached required term, stop */
                if (cache_size == n)
                    return m;
                break; /* break for-loop to continue next extension if needed */
            }
        }
    }

    /* cache_size >= n, return stored value */
    return cache[n - 1];
}
