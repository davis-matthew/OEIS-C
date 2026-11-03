/*
 * A205511  Lesser of the n-th pair of Hamming twin primes.
 * https://oeis.org/A205511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1233, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9952 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Lesser of the n‑th pair of Hamming twin primes
 * (pairs of consecutive primes with Hamming distance 1).
 * Each term is the smaller prime of such a pair.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include "../utils/next_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Hamming distance of two uint64_t numbers equals 1. */
static inline bool hamming_diff_one_uint64(uint64_t a, uint64_t b)
{
    uint64_t x = a ^ b;         /* bits that differ */
    unsigned int cnt = 0;       /* number of differing bits */
    while (x) {
        x &= x - 1;             /* drop lowest set bit */
        ++cnt;
        if (cnt > 1)            /* early exit if more than one bit differs */
            return false;
    }
    return cnt == 1;
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *terms = NULL;    /* array of seq_t values */
    static size_t cap = 0;         /* current capacity of the array */
    static idx_t count = 0;        /* number of terms stored */
    static uint64_t p = 0;         /* current prime */
    static uint64_t next = 0;      /* next prime */

    /* Initialise cache on first call. */
    if (!terms) {
        cap   = 16;
        terms = seq_alloc(cap);
        count = 0;
        p     = 2;
        next  = next_prime(p);
    }

    /* Generate terms until we have the requested index. */
    while (count < n) {
        if (hamming_diff_one_uint64(p, next)) {
            if ((size_t)count >= cap) {
                size_t newcap = cap * 2;
                seq_t *tmp    = seq_realloc(terms, cap, newcap);
                if (!tmp) /* allocation failure – abort in a sane way */
                    abort();
                terms = tmp;
                cap   = newcap;
            }
            terms[count++] = (seq_t)p;
        }
        /* Advance the prime pair. */
        p    = next;
        next = next_prime(p);
    }

    return terms[n - 1];
}
