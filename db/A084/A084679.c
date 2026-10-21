/*
 * A084679  Composite numbers with coprime numbers of prime factors with and
 *          without repetition.
 * https://oeis.org/A084679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, count_distinct_primes, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth composite number m such that
 * ω(m) > 1 and gcd(Ω(m), ω(m)) = 1,
 * where ω(m) is the number of distinct prime factors and
 * Ω(m) is the total number of prime factors (with multiplicity). */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* stored valid terms */
    static size_t cache_sz = 0;      /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t next_test = 4;      /* next integer to test */

    /* Ensure the cache array can hold at least n terms */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (cache_sz < (size_t)n) {
        /* Skip primes and numbers with only one distinct prime factor */
        if (!is_prime(next_test) &&
            count_distinct_primes((uint64_t)next_test) > 1) {
            int omega = count_distinct_primes((uint64_t)next_test);
            seq_t total = bigomega((idx_t)next_test);
            if (gcd((seq_t)omega, total) == 1) {
                cache[cache_sz++] = (seq_t)next_test;
            }
        }
        ++next_test;
    }

    return cache[n - 1];
}
