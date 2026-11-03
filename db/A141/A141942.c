/*
 * A141942  Primes congruent to 19 mod 25.
 * https://oeis.org/A141942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 19 (mod 25)
 * Range:     u8 n <= 1, u16 n <= 325, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 985 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

static seq_t *cache = NULL;        /* dynamically allocated array of terms */
static size_t cache_len = 0;       /* number of terms currently stored */
static size_t cache_cap = 0;       /* allocated capacity of the array */
static seq_t next_candidate = 19;  /* next number ≡ 19 (mod 25) to test */

/* Add terms until the array contains at least n elements. */
static void gather_until(idx_t n)
{
    while (cache_len < n) {
        /* grow the array if necessary */
        if (cache_len + 1 > cache_cap) {
            ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        }

        /* find the next prime ≡ 19 (mod 25) */
        while (true) {
            if (is_prime_seq(next_candidate)) {
                cache[cache_len++] = next_candidate;
                next_candidate += 25;              /* prepare for the next search */
                break;
            }
            next_candidate += 25;                  /* skip to the next candidate */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)        /* sequence is 1‑based; behaviour for n=0 is undefined */
        return 0;

    gather_until(n);
    return cache[n - 1];
}
