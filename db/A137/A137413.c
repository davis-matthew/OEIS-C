/*
 * A137413  a(n) = the number of positive integers that are <= n and are
 *          coprime to (the sum of the distinct prime divisors of n).
 * https://oeis.org/A137413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 1 ≤ k ≤ n | gcd(k, S(n)) = 1 } where S(n) = sum of
 *            distinct prime divisors of n (for n = 1, S(1) = 0 and a(1) = 1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   calc_phi, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"

/* Cache: cache[n] holds a(n) for n ≥ 0, cache[0] is unused.
   0 denotes “not yet computed” (a(n) is never 0 for n ≥ 1). */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    /* Ensure cache big enough */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;          /* include index n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();                  /* allocation failed */
        /* initialise new slots to 0 (reserved value) */
        for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
    }
    if (cache[n] != 0) return cache[n];           /* cached result */

    /* ----- factor n to get distinct primes ----- */
    uint64_t primes[64];      /* 64 is ample for any 64‑bit n      */
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* ----- compute S(n) ----- */
    uint64_t S = 0;
    for (size_t i = 0; i < cnt; ++i) S += primes[i];

    seq_t result;
    if (S == 0) {                       /* only possible for n = 1   */
        result = 1;                     /* only k = 1 is coprime to 0 */
    } else {
        /* ----- Euler’s totient of S ----- */
        seq_t phi_S = calc_phi((seq_t)S);

        /* ----- count full cycles ----- */
        seq_t cycles = (seq_t)(n / S);
        result = cycles * phi_S;

        /* ----- remaining part ----- */
        uint64_t r = n % S;
        unsigned partial = 0;
        for (uint64_t k = 1; k <= r; ++k)
            if (gcd((seq_t)k, (seq_t)S) == 1)
                ++partial;
        result += (seq_t)partial;
    }

    cache[n] = result;                    /* memoise */
    return result;
}
