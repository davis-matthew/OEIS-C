/*
 * A051648  Primes of form 210*p + 1 where p is a prime.
 * https://oeis.org/A051648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 25, u32 n <= 31, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051648: primes of the form 210*p + 1, where p is a prime.
   Computed by searching successive primes p, testing whether 210*p+1
   is prime, and caching the resulting values. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x);          */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n);      */

seq_t a(idx_t n) {
    /* static cache of results (1‑based index) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated size of cache */
    static size_t sz    = 0;     /* number of terms stored   */
    static idx_t  next_p = 2;    /* next candidate prime p  */

    if (n == 0) {
        /* offset is 1; n==0 is not used, return 0 as sentinel */
        return (seq_t)0;
    }

    /* generate terms until we have at least n of them */
    while (sz < (size_t)n) {
        /* find next prime p */
        idx_t p = next_p;
        while (!is_prime(p)) {
            ++p;
        }
        next_p = p + 1;          /* start after this prime next time */

        /* candidate q = 210*p + 1 */
        seq_t q = (seq_t)210 * (seq_t)p + (seq_t)1;

        /* if q is prime, cache it */
        if (is_prime_big(q)) {
            if (sz == cap) {     /* grow cache */
#ifdef __cplusplus
                size_t new_cap = (cap == 0) ? 1 : cap * 2;
                seq_t *tmp = new seq_t[new_cap];
                for (size_t i = 0; i < sz; ++i)
                    tmp[i] = cache[i];
                delete[] cache;
                cache = tmp;
                cap   = new_cap;
#else
                size_t new_cap = (cap == 0) ? 1 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
#endif
            }
            cache[sz++] = q;
        }
        /* otherwise, discard q and continue searching */
    }

    /* return requested term (1‑based) */
    return cache[(size_t)n - 1];
}
