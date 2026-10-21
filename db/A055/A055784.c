/*
 * A055784  Primes q of form q = 10p + 9, where p is also prime.
 * https://oeis.org/A055784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime q such that q = 10*p + 9 where p is prime
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(uint64_t); */

seq_t a(idx_t n)   /* 1-based index */
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;   /* array of found q's */
    static idx_t cache_len = 0;   /* number of terms stored so far */
    static uint64_t next_p = 2;   /* next candidate integer to test as p */
    static size_t cache_cap = 0;  /* current allocation capacity */

    /* Generate terms until we have at least n */
    if (n > cache_len) {
        while (cache_len < n) {
            /* Find next suitable p */
            for (;;) {
                if (is_prime(next_p)) {
                    uint64_t q_val = 10ULL * next_p + 9ULL;
                    if (is_prime(q_val)) {
                        /* Ensure enough space in cache */
                        if (cache_len >= cache_cap) {
                            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                            if (!tmp) abort();   /* out of memory */
                            cache = tmp;
                            cache_cap = new_cap;
                        }
                        cache[cache_len++] = (seq_t)q_val;
                        next_p++;   /* move on to next candidate p */
                        break;      /* term stored, go back to while */
                    }
                }
                next_p++;          /* try next integer */
            }
        }
    }

    return cache[n - 1];
}
