/*
 * A177117  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=4, k=-2 and l=0.
 * https://oeis.org/A177117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4; a(n+1)=sum_{p=0}^{n} (a(p)*a(n-p)+k)+l, with k =
 *            -2 and l = 0. This implements the recurrence a(n+1) =
 *            Σ_{p=0}^{n} a(p)*a(n-p) - 2*(n+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* dynamic cache of computed values */
    static size_t cap = 0;        /* current allocation size in elements   */
    static idx_t highest = 0;     /* highest index that has been computed  */

    /* Ensure there is space for indices 0 .. n */
    if (n >= cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* fatal: out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* initialise first terms if this is the first call */
    if (highest == 0) {
        cache[0] = (seq_t)1;            /* a(0) = 1 */
        if (cap > 1) cache[1] = (seq_t)4;/* a(1) = 4 */
        highest = 1;
    }

    /* Compute terms up to n */
    for (idx_t i = highest + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* convolution: Σ_{p=0}^{i-1} a(p)*a(i-1-p) */
        for (idx_t p = 0; p < i; ++p) {
            sum += cache[p] * cache[i - 1 - p];
        }
        /* subtract 2*(i) because k = -2 and the sum contains i terms */
        sum -= (seq_t)2 * (seq_t)i;
        cache[i] = sum;
        highest = i;
    }

    return cache[n];
}
