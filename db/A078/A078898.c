/*
 * A078898  Number of times the smallest prime factor of n is the smallest
 *          prime factor for numbers <= n; a(0)=0, a(1)=1.
 * https://oeis.org/A078898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9575 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A078898: a(n) = number of m ≤ n such that
 * smallest_prime_factor(m) = smallest_prime_factor(n).
 * a(0)=0, a(1)=1.
 * The implementation builds the values progressively and memoises
 * them so that repeated calls are O(1) after initialisation.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Ensure that 'cap' is at least 'needed', reallocating if necessary. */
#ifdef __cplusplus
template <class T>
static void _ensure_cap(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t old = *cap;
        size_t newcap = *cap ? *cap * 2 : 1;
        if (newcap < needed) newcap = needed;
        T *newptr = mem_resize(T, *ptr, (newcap * elem_size) / sizeof(T));
        if (!newptr) abort(); /* out of memory, terminate */
        if (newcap > old) {
            /* zero newly allocated memory */
            mem_set((char *)newptr + old * elem_size, 0,
                   (newcap - old) * elem_size);
        }
        *cap = newcap;
        *ptr = newptr;
    }
}
#else
static void _ensure_cap(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t old = *cap;
        size_t newcap = *cap ? *cap * 2 : 1;
        if (newcap < needed) newcap = needed;
        void *newptr = realloc(*ptr, newcap * elem_size);
        if (!newptr) abort(); /* out of memory, terminate */
        if (newcap > old) {
            /* zero newly allocated memory */
            memset((char *)newptr + old * elem_size, 0,
                   (newcap - old) * elem_size);
        }
        *cap = newcap;
        *ptr = newptr;
    }
}
#endif

/* Helper: compute smallest prime factor of n (with n ≤ 10⁴). */
static inline idx_t _spf(idx_t n)
{
    if (n < 2) return 1;            /* by definition lpf(1)=1 */
    for (idx_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return d;
    return n;                       /* n is prime */
}

/* Memoised storage for a(n) */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;        /* allocated size of a_cache */
static seq_t *prim_cnt = NULL; /* count of numbers with each prime lpf */
static size_t pcap = 0;        /* allocated size of prim_cnt */
static idx_t computed_max = 0; /* largest n processed so far */

seq_t a(idx_t n)
{
    /* Initialise caches if first call */
    if (!a_cache) {
        _ensure_cap(n + 1, &a_cap, OEISC_VPP(&a_cache), sizeof(seq_t));
        a_cache[0] = 0;              /* a(0)=0 */
        computed_max = 0;
    }
    if (!prim_cnt) {
        _ensure_cap(n + 1, &pcap, OEISC_VPP(&prim_cnt), sizeof(seq_t));
    }

    /* Extend caches to hold up to index n */
    if (n + 1 > a_cap) _ensure_cap(n + 1, &a_cap, OEISC_VPP(&a_cache), sizeof(seq_t));
    if (n + 1 > pcap) _ensure_cap(n + 1, &pcap, OEISC_VPP(&prim_cnt), sizeof(seq_t));

    /* Compute values up to n if needed */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        idx_t p = _spf(i);                 /* smallest prime factor of i */
        prim_cnt[p] = (seq_t)(prim_cnt[p] + (seq_t)1);
        a_cache[i] = prim_cnt[p];
    }
    computed_max = n;

    return a_cache[n];
}
