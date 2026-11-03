/*
 * A037933  Trajectory of 34 under prime factor concatenation procedure.
 * https://oeis.org/A037933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, tabf, base
 * Formula:   trajectory of 34 under prime factor concatenation procedure
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: full, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   factor_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function computes the nth term of the sequence where we start
   at the number 34, repeatedly factor the current number and append
   its prime factors (with multiplicity) to the output sequence, and
   then set the next current number to the decimal concatenation of
   those prime factors.  The sequence eventually reaches the fixed
   point 31397 and then repeats it.
*/

#include <stdlib.h>
#include <stdint.h>

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    /* Static workspace for the generated sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static seq_t current = (seq_t)34; /* current composite to factor */

    /* Initialise cache on first entry */
    if (!cache_len) {
        ensure_capacity(1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = current;
        cache_len = 1;
    }

    /* Generate terms until we have at least n+1 terms */
    while (cache_len <= (size_t)n) {
        /* Factor current value */
        size_t num_factors;
        uint64_t primes[64];
        uint64_t exps[64];

        /* current fits in uint64_t for this sequence */
        factor_u64((uint64_t)current, primes, exps, &num_factors);

        /* Count total prime instances to be added */
        size_t group_size = 0;
        for (size_t i = 0; i < num_factors; ++i) {
            group_size += exps[i];
        }

        /* Ensure cache has room for the new group of primes */
        ensure_capacity(cache_len + group_size, &cache_cap,
                        OEISC_VPP(&cache), sizeof(seq_t));

        /* Concatenate the primes to form the next composite */
        seq_t next = (seq_t)0;
        for (size_t i = 0; i < num_factors; ++i) {
            seq_t p = (seq_t)primes[i];
            for (size_t rep = 0; rep < exps[i]; ++rep) {
                /* Append prime to cache */
                cache[cache_len++] = p;

                /* Expanding next = next * 10^digits(p) + p */
                idx_t d = (idx_t)primes[i] ? num_digits_idx((idx_t)primes[i]) : 1;
                seq_t pow = pow10(d);
                next = next * pow + p;
            }
        }

        current = next;
    }

    return cache[(size_t)n];
}
