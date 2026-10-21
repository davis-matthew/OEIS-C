/*
 * A081534  Numbers k such that 1 + 2 ... + k divides lcm(1,2,...,k), or, in
 *          other words, lcm(1,2,...,k) is a multiple of the k-th triangular
 *          number.
 * https://oeis.org/A081534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = the n-th integer k such that the k‑th triangular 
    number k(k+1)/2 divides lcm(1,2,…,k).  Equivalently, all odd k and
    the even k for which k+1 is neither prime nor a prime power. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/count_distinct_primes.h"

/* Cache of computed terms (1‑based values). */
static seq_t *cache = NULL;       /* NULL or array of seq_t */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of terms stored */

/* Next integer k to test.  k starts at 1. */
static idx_t next_k = 1;

/* Append a new term to the cache, extending storage if necessary. */
static void append_term(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(*new_cache));
        if (!new_cache) abort();                     /* out of memory */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term of A081534 for any n ≥ 1, in any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence is 1‑indexed */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (next_k & 1ULL) {               /* odd k always qualifies */
            append_term((seq_t)next_k);
        } else {                           /* even k: test k+1 */
            uint64_t kplus1 = (uint64_t)next_k + 1;
            if (count_distinct_primes(kplus1) >= 2)
                append_term((seq_t)next_k);
        }
        ++next_k;                          /* advance to next k */
    }

    return cache[n - 1];
}
