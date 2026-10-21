/*
 * A070673  Smallest m in range 2..n-1 such that m^8 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m in range 2..n-1 such that m^8 ≡ 1 (mod n), or
 *            1 if no such m exists
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mod_pow.h"   /* for mod_pow if desired, not used here */

/* Static cache for memoization */
static seq_t *cache = NULL;
static size_t cap = 0;          /* current allocation size (>= index + 1) */
static idx_t computed_max = 0;  /* highest index already computed */

/* Helper to extend cache as needed */
static void ensure_cache(idx_t n)
{
    /* ensure array has space for indices 0..n (inclusive) */
    if (n >= cap) {
        /* double until large enough */
        size_t new_cap = cap > 1 ? cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        /* allocate or reallocate */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        /* zero newly allocated part */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* safety for out‑of‑bounds calls */

    ensure_cache(n);

    /* if value already known, return it */
    if (n <= computed_max && cache[n] != (seq_t)0)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t result = (seq_t)1;   /* default for "none found" */
        if (i <= 2) {
            /* for n = 1 or 2 there is no m in 2..n-1 */
            result = (seq_t)1;
        } else {
            seq_t mod = (seq_t)i;
            for (idx_t m = 2; m < i; ++m) {
                seq_t t = (seq_t)m;
                /* compute t^8 mod mod using 4 squarings */
                seq_t t2 = (t * t) % mod;   /* t^2 */
                seq_t t4 = (t2 * t2) % mod; /* t^4 */
                seq_t t8 = (t4 * t4) % mod; /* t^8 */
                if (t8 == (seq_t)1) {
                    result = (seq_t)m;
                    break;
                }
            }
        }
        cache[i] = result;
    }
    computed_max = n;
    return cache[n];
}
