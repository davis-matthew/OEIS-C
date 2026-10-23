/*
 * A136775  Number of multiplex juggling sequences of length n, base state
 *          <1,1> and hand capacity 2.
 * https://oeis.org/A136775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)-5*a(n-2) for n>3; Initial values: a(1)=1,
 *            a(2)=3, a(3)=11, a(4)=40
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t computed = 0;  /* highest index for which cache is defined */

    if (n == 0) return 0;   /* undefined for offset but safe */

    /* Ensure we have space for indices up to n */
    if (cap < n + 1) {
        size_t new_cap = cap ? cap * 2 : 5;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize base values on first use */
    if (computed == 0) {
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 11;
        cache[4] = 40;
        computed = 4;
    }

    /* Fill in any missing terms up to n */
    while (computed < n) {
        cache[computed + 1] = 5 * cache[computed] - 5 * cache[computed - 1];
        ++computed;
    }

    return cache[n];
}
