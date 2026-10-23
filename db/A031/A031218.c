/*
 * A031218  Largest prime power <= n.
 * https://oeis.org/A031218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A031218: largest prime power <= n
   Computed by sieving primes up to n and marking their powers. */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static idx_t cache_size = 0;
static seq_t *cache = NULL;

/* ------------------------------------------------------------------ */
/*  a(n) – largest prime power not exceeding n                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* Offset is 1 */

    /* Grow cache if necessary */
    if ((idx_t)cache_size < n) {
        size_t new_len = (size_t)(n + 1);     /* array size = n+1  */
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failure */

        /* Initialise to zero and set a(1) = 1   */
        for (size_t i = 0; i < new_len; ++i) new_cache[i] = 0;
        new_cache[1] = 1;

        /* --- Sieve primes up to n -------------------------------- */
        uint64_t *prime_list = NULL;
        size_t prime_cnt = 0;
        build_primes_upto((uint64_t)n, &prime_list, &prime_cnt);

        /* --- Mark all prime powers ------------------------------ */
        seq_t limit = (seq_t)n;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            seq_t p      = (seq_t)prime_list[pi];
            seq_t power  = p;
            while (power <= limit) {
                new_cache[(idx_t)power] = power;   /* store the power */
                if (power > limit / p) break;      /* avoid overflow   */
                power *= p;
            }
        }
        free(prime_list);

        /* --- Fill gaps with previous largest power -------------- */
        for (idx_t i = 2; i <= n; ++i)
            if (new_cache[(idx_t)i] == 0)
                new_cache[(idx_t)i] = new_cache[(idx_t)(i-1)];

        /* --- Replace old cache --------------------------------- */
        free(cache);
        cache      = new_cache;
        cache_size = n;
    }

    return cache[(idx_t)n];
}
