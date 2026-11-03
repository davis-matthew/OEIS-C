/*
 * A102546  Numbers t such that t1 is prime and t is a multiple of 10.
 * https://oeis.org/A102546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 964 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) are the multiples of 10 such that the integer obtained by
   concatenating t and 1 (i.e. 10*t + 1) is prime.  The function builds the
   sequence by successive search.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t size  = 0;
    /* Candidate multiple index (m such that t = 10*m) */
    static idx_t m_candidate = 1;

    /* Lazy initialization of the cache */
    if (cache == NULL) {
        cap   = 64;
        cache = seq_malloc(cap);
    }

    /* Generate terms until we have at least n elements */
    while (size < n) {
        /* t = 10 * m_candidate, p = 10*t + 1 */
        seq_t t = (seq_t)10 * (seq_t)m_candidate;
        seq_t p = (seq_t)10 * t + 1;

        /* Test primality of p (works for arbitrary-precision types) */
        if (is_prime_seq(p)) {
            /* Grow the cache if necessary */
            if (size == cap) {
                idx_t new_cap = cap << 1;
                cache = seq_realloc(cache, new_cap);
                cap = new_cap;
            }
            cache[size++] = t;
        }
        ++m_candidate;
    }

    return cache[n - 1];
}
