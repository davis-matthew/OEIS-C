/*
 * A084290  Difference between consecutive primes arising before difference
 *          (d=2) between twin primes. In A001223, terms before the ones that
 *          equal 2.
 * https://oeis.org/A084290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9373 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the difference preceding each twin prime gap (the gap that
 * appears immediately before a gap of size 2 in the prime‑gap sequence).
 * The function generates primes on the fly, keeps track of the preceding
 * gap, and stores each preceding gap that follows a twin prime gap in a
 * static cache so that any index can be retrieved in constant time. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
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
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined index – return 0 */

    /* Static cache for the sequence values. */
    static seq_t *gap_cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* State used while generating primes. */
    static uint64_t prev_prime = 0;
    static seq_t last_gap = 0;
    static uint64_t candidate  = 2;  /* next number to test for primality */

    /* Generate primes until we have at least n values cached. */
    while (cache_len < (size_t)n)
    {
        /* Find the next prime. */
        while (!is_prime(candidate))
            ++candidate;

        if (prev_prime != 0)
        {
            seq_t gap = (seq_t)(candidate - prev_prime);

            if (gap == (seq_t)2)  /* twin prime pair discovered */
            {
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    ensure_capacity(new_cap, &cache_cap,
                                    OEISC_VPP(&gap_cache), sizeof(seq_t));
                }
                gap_cache[cache_len++] = last_gap;  /* record preceding gap */
            }
            last_gap = gap;  /* update last gap for next iteration */
        }

        prev_prime = candidate;  /* move to the new prime */
        ++candidate;
    }

    return gap_cache[n - 1];
}
