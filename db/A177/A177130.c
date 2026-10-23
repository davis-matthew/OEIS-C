/*
 * A177130  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=9, k=0 and l=1.
 * https://oeis.org/A177130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A177130
   Recurrence: a(0)=1, a(1)=9
               a(n+1) = sum_{p=0..n} a(p)*a(n-p) + 1   (k=0, l=1)   for n>=1   */

/* Standard C10: use malloc/realloc for cache, and C++17: use std::vector. */
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Helper accessors ----------------------------------------------------- */
#ifndef __cplusplus
/* C implementation ---------------------------------------------- */
static seq_t *a_cache = NULL;      /* dynamic array of seq_t values   */
static size_t cache_capacity = 0;  /* allocated size (number of slots)*/
static idx_t cached_max = 0;       /* highest index that has a value */

/* Ensure the cache can hold indices up to n (inclusive).  New slots are not
   initialized; they will be written before being read. */
static void ensure_capacity_C(idx_t n)
{
    if ((idx_t)(cache_capacity-1) >= n) return;          /* already enough */
    size_t new_cap = (size_t)(n + 1) * 2;                 /* grow factor 2  */
    seq_t *tmp = realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    cache_capacity = new_cap;
}
static inline seq_t a_at(idx_t i)      { return a_cache[i]; }
static inline void a_set(idx_t i, seq_t v) { a_cache[i] = v; }
#else
/* C++ implementation --------------------------------------------- */
static std::vector<seq_t> a_vec;
static idx_t cached_max = 0;            /* highest index that has a value */

static inline seq_t a_at(idx_t i)      { return a_vec[i]; }
static inline void a_set(idx_t i, seq_t v) { a_vec[i] = v; }
static void ensure_capacity_CPP(idx_t n)
{
    if (a_vec.size() > (size_t)n) return;
    a_vec.resize((size_t)n + 1);
}
#endif

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
#ifndef __cplusplus
    /* Initialise cache on first call. */
    if (!a_cache) {
        cache_capacity = 2;
        a_cache = calloc(cache_capacity, sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = 1;
        a_cache[1] = 9;
        cached_max = 1;
    }
    ensure_capacity_C(n);          /* make sure we can write up to n      */
#else
    /* Initialise vector on first call. */
    if (a_vec.empty()) {
        a_vec.resize(2);
        a_vec[0] = 1;
        a_vec[1] = 9;
        cached_max = 1;
    }
    ensure_capacity_CPP(n);
#endif

    /* Return immediately if already computed. */
    if (n <= cached_max) return a_at(n);

    /* Compute terms up to n, using the recurrence. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t m = i - 1;                     /* n in the recurrence */
        for (idx_t p = 0; p <= m; ++p) {
            sum += a_at(p) * a_at(m - p);
        }
        sum += 1;                             /* l = 1 */
        a_set(i, sum);
    }
    cached_max = n;
    return a_at(n);
}
