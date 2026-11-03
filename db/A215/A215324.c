/*
 * A215324  Primes congruent to {1, 2, 3, 4, 6} mod 17.
 * https://oeis.org/A215324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 957 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to {1,2,3,4,6} mod 17 (OEIS A215324) */
/* The function iteratively tests successive integers for primality and
   keeps a cache of the primes that satisfy the residue condition. */

#include <stddef.h>
#include <stdint.h>
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

static seq_t *cached_primes = NULL;   /* array of qualifying primes */
static size_t cached_cap = 0;         /* capacity of cached_primes */
static idx_t cached_len = 0;          /* number of primes stored */
static idx_t last_checked = 2;        /* next candidate to test */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, harmless */

    /* Ensure the cache contains at least n primes. */
    while (cached_len < n) {
        if (is_prime(last_checked)) {
            uint64_t r = last_checked % 17;
            if (r == 1 || r == 2 || r == 3 || r == 4 || r == 6) {
                /* Grow the cache if necessary. */
                size_t new_needed = cached_len + 1;
                if (new_needed > cached_cap) {
                    size_t new_cap = cached_cap * 2;
                    if (new_cap < new_needed) new_cap = new_needed;
                    ensure_capacity(new_needed, &cached_cap,
                                    OEISC_VPP(&cached_primes),
                                    sizeof(seq_t));
                }
                cached_primes[cached_len++] = (seq_t)last_checked;
            }
        }
        ++last_checked;
    }

    return cached_primes[n - 1];
}
