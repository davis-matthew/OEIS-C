/*
 * A142434  Primes congruent to 24 mod 49.
 * https://oeis.org/A142434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 999 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime ≡ 24 (mod 49) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
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
    static seq_t *cache = NULL;      /* array of found primes       */
    static size_t cap   = 0;         /* current capacity of cache   */
    static idx_t found  = 0;         /* number of primes stored     */
    static seq_t last_checked = 24;  /* last tested candidate       */

    /* Find more primes until we have at least n of them. */
    while (found < n) {
        last_checked += (seq_t)49;   /* next candidate ≡ 24 (mod 49) */
        /* use the provided small‑prime test (fits 64 bit values) */
        if (is_prime_seq((uint64_t)last_checked)) {
            /* grow cache if necessary */
            if (found == (idx_t)cap) {
                ensure_capacity(found + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[found++] = last_checked;
        }
    }
    return cache[n - 1];  /* 1‑based indexing in the OEIS sequence */
}
