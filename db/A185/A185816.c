/*
 * A185816  Number of iterations of lambda(n) needed to reach 1.
 * https://oeis.org/A185816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(lambda(n)) + 1, a(1) = 0 where lambda(n) is
 *            Carmichael's lambda function
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/u64_gcd.h"

/* Cache for a(n) values.  Index 0 is unused; a_cache[1] = 0. */
static seq_t *a_cache = NULL;
static size_t   cache_cap = 0;

/* Resize cache so that index n is valid.  New entries are set to 0. */
static void ensure_cap(idx_t n) {
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) exit(1);  /* allocation failure */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    a_cache = new_cache;
    cache_cap = new_cap;
    /* base value for a(1) */
    a_cache[1] = 0;
}

/* Compute Carmichael lambda for a 64‑bit integer n (n >= 1). */
static idx_t lambda_uint64(idx_t n) {
    if (n <= 1) return 1;   /* lambda(1) = 1 by convention */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   fac_cnt = 0;

    factor_u64(n, primes, exps, &fac_cnt);

    uint64_t lcm = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t term;

        if (p == 2) {
            if (e == 1)        term = 1;
            else if (e == 2)   term = 2;
            else               term = 1ULL << (e - 2);   /* 2^(e-2) for e>=3 */
        } else {
            uint64_t pe = 1;
            for (uint64_t j = 1; j < e; ++j) pe *= p;     /* p^(e-1) */
            term = (p - 1) * pe;
        }

        uint64_t g = u64_gcd(lcm, term);
        lcm = (lcm / g) * term;
    }
    return (idx_t)lcm;
}

/* Public sequence function.  No assumed call order. */
seq_t a(idx_t n) {
    if (n == 1) return 0;

    ensure_cap(n);
    if (a_cache[n] != 0) return a_cache[n];

    idx_t lam = lambda_uint64(n);
    seq_t res = a(lam) + 1;
    a_cache[n] = res;
    return res;
}
