/*
 * A142790  Primes congruent to 23 mod 60.
 * https://oeis.org/A142790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime that is congruent to 23 (mod 60)
 * Range:     u8 n <= 2, u16 n <= 414, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 978 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
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

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_candidate = 23;

/* Ensure that the cache holds at least one term (the first prime 23). */
static void init_cache(void)
{
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
        cache[0] = 23;
        cache_len = 1;
        last_candidate = 23;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not defined for n < 1, but safe guard */

    init_cache();

    while (cache_len < n)
    {
        idx_t candidate = last_candidate + 60;          /* next number ≡ 23 (mod 60) */
        for (;;)
        {
            if (is_prime(candidate))
            {
                size_t need = cache_len + 1;
                ensure_capacity(need, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
                cache[cache_len++] = (seq_t)candidate;
                last_candidate = candidate;
                break;
            }
            candidate += 60;        /* skip to next ≡ 23 mod 60 */
        }
    }

    return cache[n - 1];
}
