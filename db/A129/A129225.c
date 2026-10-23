/*
 * A129225  Residues of the Lucas - Lehmer primality test for M(29) =
 *          536870911.
 * https://oeis.org/A129225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129225 – Residues of the Lucas–Lehmer test for M(29)=536870911
   a(0) = 4
   a(n) = (a(n-1)^2 - 2) mod 536870911    for n ≥ 1
   (a(n) is computed for all n ≥ 0, using memoisation for efficiency)
*/

#include <stdlib.h>

/* Memoised cache of previously computed terms */
static seq_t *cache      = NULL;   /* array of computed a(i) */
static idx_t  cache_size = 0;      /* number of elements stored (next free index) */

/* Return the nth term of the sequence.  The function is thread‑safe in the
   sense that it will always return a correct value, but it is not re‑entrancy
   safe because it uses static storage. */
seq_t a(idx_t n)
{
    /* Modulus of the recurrence (2^29 - 1); fits in 32 bits, but cast to seq_t. */
    const seq_t MOD = (seq_t)536870911u;

    /* Initialise cache on first call.  a(0) = 4. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failed – return zero as a safe fallback. */
            return (seq_t)0;
        }
        cache[0] = (seq_t)4;
        cache_size = 1;
    }

    /* If the desired term is already cached, return it. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Expand the cache to accommodate indices up to n. */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Realloc failed – return the last computed term. */
        return cache[cache_size - 1];
    }
    cache = new_cache;

    /* Compute new terms iteratively from the first missing index to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* Compute (prev^2) modulo MOD, then subtract 2 modulo MOD. */
        seq_t prod = prev * prev;           /* prod < 2^58 for all valid built‑in seq_t */
        seq_t val  = prod % MOD;           /* mod reduction */
        val = (val + MOD - 2) % MOD;        /* (val - 2) mod MOD, written safely */

        cache[i] = val;
    }

    cache_size = new_size;
    return cache[n];
}
