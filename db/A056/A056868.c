/*
 * A056868  Numbers that are not nilpotent numbers.
 * https://oeis.org/A056868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A056868 – Numbers that are not nilpotent numbers
    a(n) is the nth integer n ≥ 1 such that there exist
    distinct primes p and q dividing n with p^k ≡ 1 mod q
    for some integer k ≤ the exponent of p in n.  */

/*  Dependencies:  "../utils/factor_u64.h"    (provides factor_u64)
    All other included headers are standard.           */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;      /* cached terms of the sequence   */
static size_t cache_size = 0;    /* number of terms cached          */
static size_t cache_cap   = 0;    /* allocated capacity              */
static idx_t   examined   = 1;    /* last integer examined / 1       */

/*  Internal routine:  does n satisfy the A056868 condition?  */
static bool is_non_nilpotent(idx_t n)
{
    /* factor n: at most 16 distinct primes are needed */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t  num_primes = 0;

    factor_u64((uint64_t)n, primes, exps, &num_primes);
    if (num_primes < 2u) return false;        /* need at least two distinct primes */

    /* for every ordered pair (p,q) of distinct prime factors:          */
    for (size_t i = 0; i < num_primes; ++i) {
        uint64_t q = primes[i];
        for (size_t j = 0; j < num_primes; ++j) {
            if (i == j) continue;            /* distinct primes only */
            uint64_t p = primes[j];
            uint64_t e = exps[j];

            uint64_t cur = p % q;
            if (cur == 1) return true;       /* k = 1 works */
            for (uint64_t k = 2; k <= e; ++k) {
                /* multiply modulo q, avoiding overflow with 128‑bit */
                unsigned __int128 mul = (unsigned __int128)cur * p;
                cur = (uint64_t)(mul % q);
                if (cur == 1) return true;
            }
        }
    }
    return false;
}

/*  Main sequence routine  */
seq_t a(idx_t n)
{
    /* lazy allocation and expansion of the cache */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure – return zero */
    }

    while (cache_size < n) {
        /* advance to the next integer */
        ++examined;
        if (is_non_nilpotent(examined)) {
            /* store the term */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)examined;
        }
    }
    return cache[n - 1];
}
