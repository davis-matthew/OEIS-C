/*
 * A142250  Primes congruent to 1 mod 43.
 * https://oeis.org/A142250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 973 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth prime ≡ 1 (mod 43) by incremental primality testing. */
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static idx_t last_candidate = 0;

    if (n == 0) return 0;  /* Undefined for offset 0, but guard anyway. */

    while (cache_len < n) {
        idx_t candidate;
        if (cache_len == 0) {
            candidate = 173;          // First prime ≡1 (mod 43)
        } else {
            candidate = last_candidate + 43;
        }

        /* Search forward for the next such prime. */
        while (!is_prime(candidate)) {
            candidate += 43;
        }

        /* Store the found prime. */
        ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[cache_len++] = (seq_t)candidate;
        last_candidate = candidate;
    }

    return cache[n - 1];
}
