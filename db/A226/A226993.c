/*
 * A226993  Primes p such that 2p - 1 and 2p + 1 are squarefree.
 * https://oeis.org/A226993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, is_squarefree, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth prime p such that both 2p−1 and 2p+1 are squarefree. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_squarefree.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* candidate integer to test next */
    static uint64_t last_candidate = 1;

    /* generate terms until we reach n */
    while (cache_len < (size_t)n) {
        last_candidate++;               /* next integer to test */

        if (last_candidate < 2) continue;    /* skip 0 and 1 */

        /* test primality */
        if (!is_prime_big((seq_t)last_candidate)) continue;

        /* compute 2p-1 and 2p+1 */
        seq_t tp_minus = ((seq_t)last_candidate) * 2 - 1;
        if (!is_squarefree(tp_minus)) continue;

        seq_t tp_plus = tp_minus + 2;      /* 2p+1 = 2p-1 + 2 */
        if (!is_squarefree(tp_plus)) continue;

        /* append term to cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *new_mem;
            if (cache == NULL) {
                new_mem = seq_alloc(new_cap);
            } else {
                new_mem = seq_realloc(cache, cache_cap, new_cap);
            }
            if (!new_mem) return 0;  /* allocation failure: abort gracefully */
            cache = new_mem;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)last_candidate;
    }

    return cache[(size_t)n - 1];
}
