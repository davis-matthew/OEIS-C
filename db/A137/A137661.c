/*
 * A137661  a(n+1) = if a(n) is prime then (number of primes so far) else
 *          (number of nonprimes so far), a(1) = 1.
 * https://oeis.org/A137661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1)= a(n) is prime ? (number of primes so far) :
 *            (number of nonprimes so far)
 * Range:     u8 n <= 331, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

/* External linkage: seq_t and idx_t are provided by the build system */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;      /* array holding a(1)...a(sz) */
    static size_t cap   = 0;      /* current allocation capacity */
    static idx_t  sz     = 0;      /* number of terms currently cached */
    static idx_t  prime_cnt   = 0; /* primes among cached terms        */
    static idx_t  nonprime_cnt= 0; /* nonprimes among cached terms     */

    if (n == 0) return 0;          /* undefined for n<1, safe return   */

    /* Ensure we have space for n terms */
    if (n > cap) {
        size_t need = (size_t)n;
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc may fail; abort if it does (build environment guarantees enough memory) */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the cache on the first call */
    if (sz == 0) {
        /* a(1) = 1 (non‑prime) */
        cache[0] = 1;
        prime_cnt   = 0;
        nonprime_cnt= 1;
        sz = 1;
    }

    /* Compute any missing terms up to index n */
    for (idx_t i = sz; i < n; ++i) {
        /* i corresponds to a(i+1); previous term is a(i) = cache[i-1] */
        seq_t prev = cache[i - 1];
        bool is_prev_prime = is_prime_big(prev);

        seq_t value = is_prev_prime ?
                        (seq_t)prime_cnt   :
                        (seq_t)nonprime_cnt;
        cache[i] = value;

        /* Update counts with the newly produced value */
        if (is_prime_big(value))
            ++prime_cnt;
        else
            ++nonprime_cnt;
    }

    sz = n;   /* cache now contains terms up to n */
    return cache[n - 1];
}
