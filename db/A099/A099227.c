/*
 * A099227  Primes of the form m^k+k, with m and k > 1.
 * https://oeis.org/A099227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 1294, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9520 (2026-11-03)
 * Helpers:   ipow_u64_idx, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form m^k + k (m > 1, k > 1) – search
 * -------------------------------------------------------------
 * a(n) returns the n‑th prime of the form m^k + k, with offset 1.
 * The function builds a cache of all such primes up to a dynamically
 * increasing bound and keeps it sorted and unique.  The cache is
 * expanded lazily until it contains at least n elements, then the
 * requested element is returned.
 *
 * Utilities from the framework are used:
 *   - pow_uint(base,exp)     : big integer exponentiation of a
 *     uint64_t base to an idx_t exponent, returning a seq_t.
 *   - isPrimeGeneric(n)     : primality test for a seq_t.
 *   - ensure_capacity(...)  : grow a malloc'd buffer.
 *   - cmp_seq_t(...)        : comparison function for seq_t (used
 *     by qsort).  The seq_t type may be std‑int or arbitrary‑precision.
 */

#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
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

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Static cache of primes of the form m^k + k (sorted, unique). */
static seq_t *prime_cache = NULL;    /* array of seq_t values */
static size_t  prime_len   = 0;      /* number of stored primes     */
static size_t  prime_cap   = 0;      /* allocated capacity */
static seq_t   last_limit  = 0;      /* largest value for which we
                                         have already generated all
                                         applicable primes           */

/* Helper: generate all primes of the form m^k + k that are > last_limit
 * and <= limit, appending them to the cache.  After inserting the new
 * primes, the whole cache is sorted and deduplicated. */
static void generate_primes_upto(seq_t limit)
{
    if (limit <= last_limit)
        return; /* nothing new to add */

    /* Produce all candidates and append those that are new and prime. */
    for (idx_t k = 2; ; ++k) {
        seq_t min_val = ipow_u64_idx((uint64_t)2, k) + k; /* m=2 gives smallest value for this k */
        if (min_val > limit)
            break; /* no further k will produce values <= limit */

        for (idx_t m = 2; ; ++m) {
            seq_t val = ipow_u64_idx((uint64_t)m, k) + k;
            if (val > limit)
                break;          /* this m (and larger m) is too big for this k */

            if (val > last_limit && isPrimeGeneric(val)) {
                /* Append val to cache, growing capacity as necessary. */
                ensure_capacity(prime_len + 1, &prime_cap,
                                OEISC_VPP(&prime_cache), sizeof(seq_t));
                prime_cache[prime_len++] = val;
            }
        }
    }

    /* Sort the entire cache (existing + new entries). */
    if (prime_len > 1)
        mem_qsort(prime_cache, prime_len, sizeof(seq_t), cmp_seq_t);

    /* Deduplicate: compact the sorted array in place. */
    if (prime_len > 1) {
        size_t uniq = 1;
        for (size_t i = 1; i < prime_len; ++i) {
            if (prime_cache[i] != prime_cache[uniq - 1]) {
                prime_cache[uniq++] = prime_cache[i];
            }
        }
        prime_len = uniq;
    }

    last_limit = limit;
}

/* Return the n-th prime of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n elements.  Expand
     * geometrically until we have enough. */
    while (prime_len < n) {
        seq_t new_limit = (last_limit == 0) ? 1000ULL
                                            : last_limit * 2ULL + 1000ULL;
        generate_primes_upto(new_limit);
    }
    return prime_cache[n - 1];
}
