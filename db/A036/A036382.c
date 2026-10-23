/*
 * A036382  Odd split numbers: have a nontrivial factorization n=ab with a and
 *          b coprime, so that L(a) + L(b) <= L(n), where L(x) = A029837(x) =
 *          ceiling(log_2(x)).
 * https://oeis.org/A036382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A036382 – odd split numbers
   The nth term is the nth odd integer n that has a non‑trivial
   coprime factorisation n = a·b with
       ceil(log₂ a) + ceil(log₂ b) ≤ ceil(log₂ n).
   This implementation builds the sequence by brute–force testing
   successive odd candidates.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* cast helpers for C and C++ */
#ifndef TO_SEQ_T
#ifdef __cplusplus
#define TO_SEQ_T(x) static_cast<seq_t>(x)
#else
#define TO_SEQ_T(x) ((seq_t)(x))
#endif
#endif

#ifndef ALLOC_RELOC
#ifdef __cplusplus
#define ALLOC_RELOC(ptr, size, type) reinterpret_cast<type*>(realloc(ptr, size))
#else
#define ALLOC_RELOC(ptr, size, type) ((type*)realloc(ptr, size))
#endif
#endif

/* ceil(log2(x)) for unsigned 64‑bit values */
static unsigned ceil_log2_u64(uint64_t x)
{
    if (x <= 1) return 0;
    unsigned bits = 0;
    uint64_t y = x - 1;
    while (y) { y >>= 1; ++bits; }
    return bits;
}

/* test whether an odd uint64_t is a split number */
static bool is_odd_split(uint64_t n)
{
    /* factorise n into distinct prime powers */
    uint64_t primes[16];
    uint64_t powers[16];
    size_t k = 0;

    uint64_t tmp = n;
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            if (k >= sizeof(primes)/sizeof(primes[0])) return false;
            uint64_t pow = 1;
            while (tmp % p == 0) {
                pow *= p;
                tmp /= p;
            }
            primes[k] = p;
            powers[k] = pow;
            ++k;
        }
    }
    if (tmp > 1) {
        if (k >= sizeof(primes)/sizeof(primes[0])) return false;
        primes[k] = tmp;
        powers[k] = tmp;
        ++k;
    }

    if (k < 2) return false;   /* need at least two distinct primes */

    unsigned Ln = ceil_log2_u64(n);

    /* try all non‑trivial coprime combinations of the prime powers */
    for (size_t mask = 1; mask + 1 < (1u << k); ++mask) {
        uint64_t a = 1;
        for (size_t i = 0; i < k; ++i) {
            if (mask & (1u << i)) a *= powers[i];
        }
        uint64_t b = n / a;
        unsigned La = ceil_log2_u64(a);
        unsigned Lb = ceil_log2_u64(b);
        if (La + Lb <= Ln) return true;
    }
    return false;
}

/* cached values of the sequence */
static seq_t  *cache     = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;

/* next odd candidate to test (starting at 21) */
static uint64_t cur_candidate = 21;

/* return a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return TO_SEQ_T(0);   /* undefined, guard */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* find next odd split number */
        while (!is_odd_split(cur_candidate)) cur_candidate += 2;

        /* enlarge cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *new_cache = ALLOC_RELOC(cache,
                                           new_cap * sizeof(seq_t),
                                           seq_t);
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* store the term */
        cache[cache_len++] = TO_SEQ_T(cur_candidate);

        /* move to the next odd candidate */
        cur_candidate += 2;
    }

    return cache[n - 1];
}
