/*
 * A171503  Number of 2 X 2 integer matrices with entries from {0,1,...,n}
 *          having determinant 1.
 * https://oeis.org/A171503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 0, a(1) = 3, a(n) = a(n-1) + 4*phi(n) for n > 1
 *            where phi is Euler's totient function
 * Range:     u8 n <= 14, u16 n <= 231, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    /* Local static cache of computed terms */
    static seq_t *cache = NULL;   /* array of terms indexed by n */
    static size_t cap = 0;        /* allocated capacity of cache */
    static idx_t last_n = (idx_t)-1; /* largest n computed so far */

    /* Ensure capacity up to n */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = NULL;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!new_cache) return 0; /* allocation failure → return 0 as fallback */
        cache = new_cache;
        cap = new_cap;
    }

    /* Initialize base cases if needed */
    if (cache[0] != cache[0]) {  /* uninitialized check (any value) */
        cache[0] = 0;
    }
    if (n == 0) return cache[0];
    if (cache[1] == 0 && n >= 1) {
        cache[1] = 3;
    }
    if (n == 1) return cache[1];

    /* Compute missing terms up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;
        } else if (i == 1) {
            cache[1] = 3;
        } else {
            seq_t phi_i = calc_phi((seq_t)i);        /* phi(i) in seq_t */
            seq_t addend = 4 * phi_i;                /* 4*phi(i) */
            cache[i] = cache[i - 1] + addend;        /* recurrence */
        }
    }
    last_n = n;
    return cache[n];
}
