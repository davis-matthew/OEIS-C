/*
 * A010212  Continued fraction for sqrt(163).
 * https://oeis.org/A010212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   alloc_seq_array, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010212: continued fraction terms for sqrt(163)
   The terms are generated using the standard continued‑fraction algorithm
   for sqrt(N): start with a0 = floor(sqrt(N)); then iterate
   m_{i+1} = d_i*a_i - m_i
           d_{i+1} = (N - m_{i+1}^2) / d_i
           a_{i+1} = floor((a0 + m_{i+1}) / d_{i+1}).
   The sequence starts with a(0) = a0 and repeats with period 18. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    /* static state for caching computed terms */
    static seq_t *cache = NULL;         /* array of terms */
    static size_t cache_cap = 0;        /* allocated capacity */
    static idx_t cache_len = 0;         /* number of computed terms */

    /* constants for sqrt(163) */
    static const seq_t N = (seq_t)163;
    static seq_t a0 = 0;   /* integer part of sqrt(N) */
    static bool initialized = false;

    /* state of the continued fraction iteration */
    static seq_t m = 0, d = 1, a_cur = 0;

    /* initialize once */
    if (!initialized)
    {
        a0 = floor_sqrt(N);
        /* ensure space for first term */
        cache_cap = 1;
        cache = alloc_seq_array(cache_cap);
        cache[0] = a0;
        cache_len = 1;

        /* initial state of the algorithm for further terms */
        m = 0;
        d = 1;
        a_cur = a0;

        initialized = true;
    }

    /* extend the cache up to index n */
    while (cache_len <= n)
    {
        /* compute next term */
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (N - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;  /* floor division */

        /* ensure capacity */
        if (cache_len >= cache_cap)
        {
            cache_cap = cache_cap * 2 + 1;
            seq_t *tmp = alloc_seq_array(cache_cap);
            for (size_t i = 0; i < cache_len; ++i)
                tmp[i] = cache[i];
            free(tmp);           /* free the old array */
            cache = tmp;
        }

        cache[cache_len] = a_next;
        cache_len++;

        /* update state for next iteration */
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return cache[n];
}
