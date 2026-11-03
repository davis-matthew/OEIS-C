/*
 * A119289  Prime numbers p such that there is no prime between 10*p and
 *          10*p+9 inclusive.
 * https://oeis.org/A119289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime p such that all numbers 10*p+d (d=0..9)
 *            are composite
 * Range:     u8 none, u16 n <= 624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9536 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    if (n == 0) return (seq_t)0; /* undefined for n<1, return 0 */

    /* static state: cache of qualifying primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;

    /* static state: last candidate checked */
    static uint64_t last_candidate = 1;

    /* grow cache if necessary */
    while (cache_cnt < (size_t)n) {
        /* find next prime */
        while (true) {
            last_candidate++;
            if (is_prime_seq(last_candidate)) { /* prime found */
                bool ok = true;
                for (uint64_t d = 0; d < 10; ++d) {
                    uint64_t x = last_candidate * 10 + d;
                    if (is_prime_seq(x)) {  /* found a prime in the range */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    /* store it */
                    if (cache_cnt >= cache_cap) {
                        ensure_capacity(cache_cnt + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
                    }
                    cache[cache_cnt++] = (seq_t)last_candidate;
                }
                break; /* either stored or skipped */
            }
        }
    }

    return cache[n - 1];
}
