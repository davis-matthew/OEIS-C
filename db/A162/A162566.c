/*
 * A162566  Those primes p where (q-p) divides (p+1), where q is the least
 *          prime > p.
 * https://oeis.org/A162566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 968 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162566: primes p such that the next prime q satisfies (q‑p)|(p+1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/next_prime.h"
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

/* Static cache of qualifying primes */
static seq_t *special_primes = NULL;
static size_t special_cap   = 0;
static size_t special_len   = 0;

/* Current prime to test and initialization flag */
static uint64_t cur_prime = 0;
static bool initialized  = false;

/* Expand the cache until we have at least 'need' terms. */
static void ensure_special(idx_t need)
{
    if (!initialized) {
        cur_prime   = 2;          /* start with the first prime */
        initialized = true;
    }

    while (special_len < (size_t)need) {
        uint64_t p = cur_prime;    /* current prime */
        uint64_t q = next_prime(p);/* least prime > p */

        /* test divisibility condition: (p+1) mod (q-p) == 0 */
        if ( ((seq_t)(p + 1)) % (q - p) == 0 ) {
            if (special_len == special_cap) {
                ensure_capacity(special_len + 1, &special_cap,
                                OEISC_VPP(&special_primes), sizeof(seq_t));
            }
            special_primes[special_len++] = (seq_t)p;
        }

        cur_prime = q; /* advance to the next prime for the next iteration */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour below offset is irrelevant */
    ensure_special(n);
    return special_primes[n - 1];
}
