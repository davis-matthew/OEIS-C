/*
 * A177490  Primes of the form (41*10^n-23)/9.
 * https://oeis.org/A177490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form (41·10^k – 23)/9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* below offset, unused */

    /* Static cache of found primes */
    static seq_t *cache = NULL;           /* array of primes */
    static size_t cache_size = 0;         /* number of primes stored */
    static size_t cache_cap  = 0;          /* capacity of the array */
    static idx_t next_k = 0;              /* current exponent k */

    /* Allocate cache on first use */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
    }

    /* Ensure sufficient capacity */
    if (cache_size >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Generate candidates until we have at least n primes */
    while (cache_size < n) {
        seq_t val = (seq_t)41 * pow10(next_k);
        val -= (seq_t)23;
        val /= (seq_t)9;                /* division is exact */

        if (is_prime_big(val)) {
            /* Store prime (grow array if necessary) */
            if (cache_size + 1 > cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = val;
        }
        ++next_k;                       /* next exponent */
    }

    return cache[n - 1];                 /* 1‑based index */
}
