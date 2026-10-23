/*
 * A171530  Denominator of (n-th nonprime/n).
 * https://oeis.org/A171530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of (A018252(n)/n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018252(n) is the nth nonprime (1,4,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,27,28,30,32,33,34,35,36,38,...) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* static cache of the first k nonprimes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* initialise with the first nonprime 1 */
    if (cache_size == 0) {
        cache_cap   = 64;
        cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = (seq_t)1;
        cache_size  = 1;
    }

    /* grow cache until it contains the nth nonprime */
    while (cache_size < n) {
        idx_t candidate = (idx_t)cache[cache_size - 1] + 1;

        /* find the next nonprime */
        while (is_prime_small((seq_t)candidate)) {
            ++candidate;
        }

        /* grow array if necessary */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap << 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache       = tmp;
            cache_cap   = new_cap;
        }

        cache[cache_size++] = (seq_t)candidate;
    }

    /* compute denominator of (nonprime(n)/n) */
    seq_t num = cache[n - 1];
    seq_t denom_gcd = gcd(num, (seq_t)n);
    seq_t denom = ((seq_t)n) / denom_gcd;
    return denom;
}
