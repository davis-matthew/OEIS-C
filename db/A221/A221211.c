/*
 * A221211  Numbers n such that n and n + 4 are prime and there is a power of
 *          two in the interval (n,n+4).
 * https://oeis.org/A221211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A221211: numbers n such that n and n+4 are prime and a power of two lies in (n,n+4). */
/* The function searches over powers of two, testing whether t-1, t-2 or t-3
   are primes with the corresponding t+3, t+2, or t+1 also prime, and
   collects the qualifying n in a memoised array. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static memoisation state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cur_pow = 0;
    static bool initialized = false;

    /* Initialise on first call */
    if (!initialized) {
        cur_pow = (seq_t)4;      /* 2^2, the first relevant power of two */
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* For each power of two t = cur_pow check t-1, t-2, t-3 */
        for (int d = 1; d <= 3; ++d) {
            seq_t p = cur_pow - (seq_t)d;
            if (p < (seq_t)2) continue;
            if (is_prime_big(p) && is_prime_big(p + (seq_t)4)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) return 0;  /* Out of memory – unlikely in tests */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = p;
                if (cache_len >= n) break;  /* stop adding more for now */
            }
        }
        cur_pow = cur_pow * (seq_t)2;
    }

    return cache[n - 1];
}
