/*
 * A177504  Primes of the form 5*10^n-9.
 * https://oeis.org/A177504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A177504: Primes of the form 5*10^n-9.
   a(1) = 41, a(2) = 491, a(3) = 49991, a(4) = 49999991, a(5) = 4999999999999999999999991,
   ...  where the exponent n is not the index but the amount of 10s in the power.
   This implementation generates primes of the form 5*10^e-9 by brute‑force search
   and memoises the results.  The helper functions pow10, is_prime_big and seq_realloc
   are supplied by the build system.  The algorithm is O(m) where m is the
   exponent of the last prime found.  It works for the known terms (usually
   a few dozen) and easily stays below the time limit for n up to 10000.
 */

#include <stdbool.h>
#include <stddef.h>               /* size_t */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e) */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n) */
#include "../utils/seq_realloc.h"/* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

seq_t a(idx_t n) {
    /* Static cache of found primes and the next exponent to check. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_exponent = 1;  /* start with 10^1 */

    /* Ensure cache is initialized with a small capacity. */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = seq_realloc(NULL, 0, cache_cap);
        cache_len = 0;
    }

    /* Generate more primes until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t ten_pow = pow10(next_exponent);         /* 10^next_exponent   */
        seq_t candidate = ten_pow * (seq_t)5 - (seq_t)9; /* 5*10^e - 9 */

        if (is_prime_big(candidate)) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = seq_realloc(cache, cache_len, cache_cap);
            }
            cache[cache_len++] = candidate;
        }
        ++next_exponent;  /* proceed to the next exponent */
    }

    return cache[n - 1];  /* cache is 0‑based, a(1) is at index 0 */
}
