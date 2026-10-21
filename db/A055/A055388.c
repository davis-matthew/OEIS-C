/*
 * A055388  Number of riffle shuffles of 2n cards required to return the deck
 *          to its initial state.
 * https://oeis.org/A055388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A055388: Number of riffle shuffles of 2n cards required to return
 *  the deck to its initial state.
 *  Formula: a(n) = 2 * min{k>0 | 4^k ≡ 1 (mod 2n-1)}.   */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1.  For smaller indices no value is
     * defined, so return 0.  All computed values are positive, so 0
     * can also be used as a sentinel in the cache. */
    if (n < 1)
        return 0;

    /* Memoisation cache: cache[i] holds a(i) for i ≥ 1.  The cache pointer
     * is expanded lazily; new entries are initialised to 0. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    if (n >= cache_cap) {
        idx_t new_cap = n + 1;                 /* indices 0 … n are needed */
        seq_t *tmp = (seq_t *)realloc(cache,
                           sizeof(seq_t) * new_cap);
        if (!tmp)
            return 0;                         /* cannot allocate – give up */

        cache = tmp;
        if (new_cap > cache_cap)
            memset(cache + cache_cap, 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    if (cache[n] != 0)           /* already computed */
        return cache[n];

    /* Special case: n = 1  → 2n-1 = 1, 4^k ≡ 1 (mod 1) holds for k=1. */
    if (n == 1) {
        cache[n] = 2;
        return 2;
    }

    /* Compute the multiplicative order of 4 modulo m = 2n-1. */
    seq_t m = (seq_t)(2ULL * n - 1ULL);
    seq_t target = 1 % m;                 /* equals 1 for m>1, 0 for m==1 */
    seq_t val = 4 % m;                    /* 4^1 mod m */
    seq_t k = 1;

    while (val != target) {               /* search for the smallest k>0 */
        val = (val * 4) % m;
        ++k;
    }

    seq_t result = 2 * k;
    cache[n] = result;
    return result;
}
