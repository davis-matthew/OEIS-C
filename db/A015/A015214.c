/*
 * A015214  Sum of Gaussian binomial coefficients for q=22.
 * https://oeis.org/A015214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 2 a(n) = 2*a(n-1) + a(n-2)*((22^(n-1)) - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* cache for a(n) */
    static seq_t *a_cache = NULL;
    static size_t a_cap = 0;
    static idx_t  a_last = 1;   /* last computed index (>=1) */

    /* cache for powers of 22: pow_cache[k] = 22^k  (k >= 0) */
    static seq_t *pow_cache = NULL;
    static size_t pow_cap = 0;
    static idx_t  pow_last = 1; /* last computed power index */

    /* trivial cases */
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* initialize caches if first call */
    if (a_cache == NULL) {
        a_cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!a_cache) return 0;        /* out of memory */
        a_cache[0] = 1;
        a_cache[1] = 2;
        a_cap = 2;
        a_last = 1;
    }
    if (pow_cache == NULL) {
        pow_cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!pow_cache) return 0;      /* out of memory */
        pow_cache[0] = 1;
        pow_cache[1] = 22;
        pow_cap = 2;
        pow_last = 1;
    }

    /* enlarge a_cache if needed */
    if (a_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory */
        a_cache = tmp;
        a_cap = new_cap;
    }

    /* enlarge pow_cache if needed (we need indices up to n) */
    if (pow_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(pow_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory */
        pow_cache = tmp;
        pow_cap = new_cap;
    }

    /* extend powers of 22 up to index n if necessary */
    for (idx_t i = pow_last + 1; i <= n; ++i) {
        pow_cache[i] = pow_cache[i - 1] * (seq_t)22;
    }
    pow_last = n;

    /* compute a(n) iteratively using stored previous values */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t term = a_cache[i - 1] * (seq_t)2
                   + a_cache[i - 2] * (pow_cache[i - 1] - (seq_t)1);
        a_cache[i] = term;
    }
    a_last = n;

    return a_cache[n];
}
