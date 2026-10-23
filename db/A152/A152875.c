/*
 * A152875  Number of permutations of {1,2,...,n} with all odd entries
 *          preceding all even entries or all even entries preceding all odd
 *          entries.
 * https://oeis.org/A152875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * floor((n+1)/2)! * floor(n/2)!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;
static size_t fact_cache_len = 0;

/* Ensure factorials 0..up_to are pre‑computed. */
static void ensure_fact(idx_t up_to)
{
    if (fact_cache == NULL) {
        size_t new_len = (size_t)up_to + 1;
        fact_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!fact_cache) return;                /* allocation failed – give up */
        fact_cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= up_to; ++i)
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        fact_cache_len = new_len;
        return;
    }
    if (fact_cache_len <= (size_t)up_to) {
        size_t new_len = (size_t)up_to + 1;
        seq_t *ptr = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
        if (!ptr) return;                      /* allocation failed – give up */
        fact_cache = ptr;
        for (idx_t i = fact_cache_len; i <= up_to; ++i)
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        fact_cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1u)
        return (seq_t)1;

    idx_t k = n / 2u;           /* floor(n/2) */

    if ((n & 1u) == 0u) {       /* even n = 2k */
        ensure_fact(k);
        return (fact_cache[k] * fact_cache[k]) * (seq_t)2;
    } else {                    /* odd n = 2k+1 */
        ensure_fact(k + 1u);
        return (fact_cache[k] * fact_cache[k + 1u]) * (seq_t)2;
    }
}
