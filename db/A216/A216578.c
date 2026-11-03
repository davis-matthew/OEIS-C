/*
 * A216578  Prime numbers which cannot be written in the form x^2 + 10*y^2.
 * https://oeis.org/A216578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes that cannot be written as x^2 + 10*y^2
   (equivalently primes not congruent to 1, 9, 11, or 19 modulo 40). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

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
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t candidate = 2;   /* start from the first prime */

    if (n == 0) return 0;  /* offset not used but safe */

    while (cache_len < (size_t)n) {
        if (is_prime(candidate)) {
            uint64_t rem = candidate % 40ULL;
            if (rem != 1ULL && rem != 9ULL && rem != 11ULL && rem != 19ULL) {
                ensure_capacity(cache_len + 1, &cache_cap,
                                OEISC_VPP(&cache), sizeof(seq_t));
                cache[cache_len] = (seq_t)candidate;
                cache_len++;
            }
        }
        candidate++;
    }
    return cache[n - 1];
}
