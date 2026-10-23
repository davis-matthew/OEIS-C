/*
 * A028261  Numbers whose total number of prime factors (counting
 *          multiplicity) is squarefree.
 * https://oeis.org/A028261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A028261: Numbers whose total number of prime factors (counting
 *  multiplicity) is square‑free.  The function a(idx_t n) returns the
 *  nth such number (a(1) = 2).  The implementation builds a running
 *  sieve of smallest prime factors and a cache of the sequence values.
 *  It works for both native integer seq_t types (in C) and for an
 *  arbitrary‑precision seq_t class (in C++17).  The sequence is computed
 *  incrementally and memoised so that calls in any order are O(1)
 *  after the required range has been generated.  */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
 
/* The following code is compiled in both C11 and C++17.  For the C++
 * build seq_t is an arbitrary‑precision integer class; for the C
 * build it is a builtin unsigned integer type.  The file can be
 * compiled as either language.  */
 
#ifdef __cplusplus
#include <vector>
#endif
 
/*---------------------------------------------------------------------------*/
/*  Helper data structures                                               */
/*---------------------------------------------------------------------------*/
 
#if defined(__cplusplus)

/* C++ build – use std::vector for automatic construction/destruction */
static std::vector<idx_t>  spfVec;          /* smallest prime factor[ ]   */
static std::vector<seq_t> cacheVec;        /* cached sequence values     */
static idx_t next_candidate = 2;           /* next integer to test       */

#else

/* C build – use malloc/realloc; seq_t is a POD (built‑in unsigned int) */
static idx_t *spf   = NULL;    /* smallest prime factor array */
static idx_t spf_limit = 0;    /* current maximum covered by spf */
static seq_t *cache = NULL;    /* cached sequence values     */
static size_t cache_len = 0;   /* number of cached terms      */
static size_t cache_cap = 0;   /* allocated capacity of cache */
static idx_t next_candidate = 2;           /* next integer to test       */

#endif
 
/*---------------------------------------------------------------------------*/
/*  Smallest prime factor table generation                                 */
/*---------------------------------------------------------------------------*/
 
static void spf_generate(idx_t limit)
{
#if defined(__cplusplus)
    /* Re‑allocate the vector to the required size */
    spfVec.assign(limit + 1, 0);
    for (idx_t i = 2; i <= limit; ++i) {
        if (spfVec[i] == 0) {
            spfVec[i] = i;
            if (i <= limit / i) {          /* avoid overflow of i*i   */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spfVec[j] == 0) spfVec[j] = i;
            }
        }
    }
#else
    free(spf);
    spf = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!spf) return;                       /* allocation failure      */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= limit / i)
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
    }
    spf_limit = limit;
#endif
}
 
static void ensure_spf(idx_t limit)
{
#if defined(__cplusplus)
    if (spfVec.size() > limit) return;
    spf_generate(limit);
#else
    if (spf_limit >= limit) return;
    spf_generate(limit);
#endif
}
 
/*---------------------------------------------------------------------------*/
/*  Count Ω(n): total number of prime factors (with multiplicity)           */
/*---------------------------------------------------------------------------*/
static seq_t count_omega(idx_t n)
{
    seq_t cnt = 0;
    while (n > 1) {
#if defined(__cplusplus)
        idx_t p = spfVec[n];
#else
        idx_t p = spf[n];
#endif
        ++cnt;
        n /= p;
    }
    return cnt;
}
 
/*---------------------------------------------------------------------------*/
/*  Test whether a small integer w is square‑free                           */
/*---------------------------------------------------------------------------*/
static bool is_squarefree_w(seq_t w)
{
    /* w is guaranteed to be very small (at most ~64 for 64‑bit n). */
    idx_t vv = (idx_t)w;
    if (vv < 2) return true;
    for (idx_t p = 2; p * p <= vv; ++p)
        if (vv % (p * p) == 0) return false;
    return true;
}
 
/*---------------------------------------------------------------------------*/
/*  Cache handling                                                         */
/*---------------------------------------------------------------------------*/
#if !defined(__cplusplus)
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) return;  /* allocation failure – leave as is       */
    cache = new_ptr;
    cache_cap = new_cap;
}
#endif
 
/*---------------------------------------------------------------------------*/
/*  Generate sequence terms until at least n terms are cached              */
/*---------------------------------------------------------------------------*/
static void generate_until(size_t n)
{
    size_t len =
#if defined(__cplusplus)
        cacheVec.size();
#else
        cache_len;
#endif
    if (len >= n) return;
 
    while (len < n) {
        idx_t candidate = next_candidate;
        ensure_spf(candidate);
        seq_t omega = count_omega(candidate);
        if (is_squarefree_w(omega)) {
#if defined(__cplusplus)
            cacheVec.push_back((seq_t)candidate);
#else
            ensure_cache_capacity(len + 1);
            cache[len] = (seq_t)candidate;
            ++cache_len;
#endif
            ++len;
        }
        ++next_candidate;
    }
}
 
/*---------------------------------------------------------------------------*/
/*  Public sequence function                                                */
/*---------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; safety value   */
    generate_until((size_t)n);
#if defined(__cplusplus)
    return cacheVec[(size_t)(n - 1)];
#else
    return cache[(size_t)(n - 1)];
#endif
}
