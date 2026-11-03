/*
 * A118747  a(n) = product[k=1..n] P(k), where P(k) is the largest prime <=
 *          2*k. a(n) = product[k=1..n] A060308(k).
 * https://oeis.org/A118747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1..n} P(k) where P(k) is the largest prime ≤ 2·k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
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

/* Memoized cache of a(k) values */
static seq_t *a_cache      = NULL; /* a_cache[i] = a(i) for 0≤i≤current_max */
static size_t a_cache_cap  = 0;
static idx_t  current_max  = 0;

/* Cached list of primes up to the current prime limit */
static uint64_t *prime_list  = NULL;
static size_t    prime_cnt   = 0;
static idx_t     prime_limit = 0; /* the maximum 2·k for which primes are cached */

/* Ensure that prime_list contains all primes ≤ limit. */
static void
ensure_prime_list(idx_t limit)
{
    idx_t needed = limit << 1; /* 2*limit */
    if (needed <= prime_limit)
        return; /* already have enough */

    /* Build a new list of primes up to needed */
    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(needed, &new_list, &new_cnt);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_limit = needed;
}

/* Binary search in prime_list for the largest prime ≤ target */
static uint64_t
largest_prime_leq(idx_t target)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= (uint64_t)target)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is first index with prime > target */
    return (lo == 0) ? 0 : prime_list[lo - 1];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1; /* not part of OEIS but defensive */

    /* Initialise cache on first call */
    if (a_cache == NULL) {
        ensure_capacity((size_t)1, &a_cache_cap, OEISC_VPP(&a_cache), sizeof(seq_t));
        a_cache[0] = 1;          /* ∏ over empty set */
        current_max = 0;
    }

    if (n <= current_max)        /* already computed */
        return a_cache[n];

    /* Make sure we have all primes up to 2*n */
    ensure_prime_list(n);

    /* Grow cache if necessary */
    if (a_cache_cap <= (size_t)n) {
        ensure_capacity((size_t)n + 1, &a_cache_cap, OEISC_VPP(&a_cache), sizeof(seq_t));
    }

    /* Compute missing values */
    for (idx_t k = current_max + 1; k <= n; ++k) {
        idx_t two_k = k << 1;
        uint64_t p  = largest_prime_leq(two_k);
        a_cache[k] = a_cache[k - 1] * (seq_t)p;
    }

    current_max = n;
    return a_cache[n];
}
