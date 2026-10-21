/*
 * A218033  G.f. A(x) satisfies A(x) = 1 + x * A(x) / A(x^2).
 * https://oeis.org/A218033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 49, i32 n <= 96, i64 n <= 196,
 *            i128 n <= 394, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218033.
   A(x) = 1 + x*A(x)/A(x^2).
   Let a(0)=1.  For n≥1,
   let S(n)= Σ_{j=1..⌊n/2⌋} a(n-2j)*a(j).

   If n is odd:
        a(n) = a(n-1) - S(n).

   If n is even:
        a(n) = a(n-1) + a(n/2) - S(n).
   All arithmetic is performed in type seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache that grows as needed */
    static seq_t *cache      = NULL;
    static idx_t  capacity   = 0;
    static idx_t  computed   = 0;

    /* initialise cache on first call */
    if (capacity == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;          /* a(0) = 1 */
        capacity = 1;
        computed  = 0;
    }

    /* enlarge cache if required */
    if (n >= capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing values up to index n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t tmp = 0;
        idx_t maxj = k / 2;
        for (idx_t j = 1; j <= maxj; ++j) {
            tmp += cache[k - 2 * j] * cache[j];
        }
        if ((k & 1) == 0) {                    /* k is even */
            cache[k] = cache[k - 1] + cache[k / 2] - tmp;
        } else {                                /* k is odd */
            cache[k] = cache[k - 1] - tmp;
        }
    }

    computed = n;
    return cache[n];
}
