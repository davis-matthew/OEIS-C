/*
 * A072885  Primes p with a prime number of digits in all base b with 1 <= b
 *          <= p.
 * https://oeis.org/A072885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime, is_prime_u32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate primes p such that the number of digits of p in every base
   b with 1 <= b <= p is a prime.  This is OEIS A072885. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>          /* for sqrt in case size_t > integer, only used for cast */
#include "../utils/is_prime.h"
#include "../utils/is_prime_u32.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

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

/* Check whether a prime p has a composite digit length in some base 2 <= b <= p. */
static bool _has_composite_digit_length(idx_t p)
{
    /* Only bases up to sqrt(p) can produce length > 2. */
    idx_t limit = (idx_t)floor(sqrt((double)p));
    for (idx_t b = 2; b <= limit; ++b)
    {
        /* Compute length of p in base b. */
        idx_t tmp = p;
        unsigned int len = 0;
        while (tmp)
        {
            tmp /= b;
            ++len;
        }
        if (!is_prime_u32(len))
            return true; /* composite length found */
    }
    return false; /* all considered lengths are prime */
}

/* Main sequence generator with memoisation. */
seq_t a(idx_t n)
{
    static bool          _initialized = false;
    static idx_t         _next_candidate = 2;
    static seq_t       * _seq_cache = NULL;
    static size_t        _seq_cap   = 0;
    static size_t        _seq_len   = 0;

    if (!_initialized)
    {
        _seq_cap = 1024;
        _seq_cache = mem_new(seq_t, _seq_cap);
        _seq_len = 0;
        _initialized = true;
    }

    while (_seq_len < n)
    {
        if (is_prime(_next_candidate))
        {
            idx_t p = _next_candidate;
            if (!_has_composite_digit_length(p))
            {
                ensure_capacity(_seq_len + 1, &_seq_cap,
                        OEISC_VPP(&_seq_cache), sizeof(seq_t));
                _seq_cache[_seq_len++] = (seq_t)p;
            }
        }
        ++_next_candidate;
    }

    return _seq_cache[n - 1];
}
