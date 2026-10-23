/*
 * A039819  Number of divisors of n-th refactorable number (A033950(n)).
 * https://oeis.org/A039819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of the n‑th refactorable number
 *            (A033950(n)). The value is obtained by scanning natural numbers,
 *            counting those for which τ(k) divides k, and taking τ of the
 *            requested one
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"   /* divisor_count_u64 */
#include "../utils/seq_alloc.h"          /* seq_alloc */
#include "../utils/seq_realloc.h"        /* seq_realloc */

static seq_t *a_cache     = NULL;   /* cache of τ values for refactorables */
static size_t  cache_cap  = 0;      /* capacity of a_cache              */
static size_t  cache_size = 0;      /* number of computed values         */
static uint64_t last_checked = 1;   /* next integer to test             */

/* Factor count helper: returns divisor count of n (τ(n)) */
static inline seq_t tau(uint64_t n)
{
    return divisor_count_u64(n);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, safe fallback */

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size)
        return a_cache[(size_t)n - 1];

    /* Ensure initial capacity */
    if (a_cache == NULL) {
        cache_cap = 64;
        a_cache = seq_alloc(cache_cap);
    }

    /* Scan integers until we reach the n‑th refactorable number */
    for (uint64_t i = last_checked; ; ++i) {
        seq_t t = tau(i);
        uint64_t t64 = (uint64_t)t;
        if (t64 != 0 && i % t64 == 0) {      /* i is refactorable */
            if (cache_size == cache_cap) {   /* grow cache */
                size_t new_cap = cache_cap * 2;
                a_cache = seq_realloc(a_cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            a_cache[cache_size++] = t;
            if ((size_t)n <= cache_size) {
                last_checked = i + 1;
                return a_cache[(size_t)n - 1];
            }
        }
    }
}
