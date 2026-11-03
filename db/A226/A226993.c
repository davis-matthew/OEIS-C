/*
 * A226993  Primes p such that 2p - 1 and 2p + 1 are squarefree.
 * https://oeis.org/A226993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 963 (2026-11-03)
 * Helpers:   is_prime_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth prime p such that both 2p−1 and 2p+1 are squarefree. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* candidate integer to test next */
    static uint64_t last_candidate = 1;

    /* generate terms until we reach n */
    while (cache_len < (size_t)n) {
        last_candidate++;               /* next integer to test */

        if (last_candidate < 2) continue;    /* skip 0 and 1 */

        /* test primality */
        if (!is_prime_big((seq_t)last_candidate)) continue;

        /* compute 2p-1 and 2p+1 */
        seq_t tp_minus = ((seq_t)last_candidate) * 2 - 1;
        if (!is_squarefree(tp_minus)) continue;

        seq_t tp_plus = tp_minus + 2;      /* 2p+1 = 2p-1 + 2 */
        if (!is_squarefree(tp_plus)) continue;

        /* append term to cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *new_mem;
            if (cache == NULL) {
                new_mem = seq_alloc(new_cap);
            } else {
                new_mem = seq_realloc(cache, cache_cap, new_cap);
            }
            if (!new_mem) return 0;  /* allocation failure: abort gracefully */
            cache = new_mem;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)last_candidate;
    }

    return cache[(size_t)n - 1];
}
