/*
 * A100305  Expansion of (1 - x - 4*x^2)/(1 - 2*x - 7*x^2 + 8*x^3).
 * https://oeis.org/A100305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 8*a(n-3); Initial values: a(0)=1,
 *            a(1)=1, a(2)=5
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: capacity and number of computed entries */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;

    /* initialise with first three terms */
    if (cap == 0) {
        cap = 4;                          /* enough for indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        len   = 3;
    }

    /* If n is already computed, return it */
    if (n < len) {
        return cache[n];
    }

    /* Ensure capacity is large enough for index n */
    if (n >= cap) {
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 4;
        while (new_cap <= n) {
            new_cap = new_cap * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i - 1] + (seq_t)7 * cache[i - 2];
        val = val - (seq_t)8 * cache[i - 3];
        cache[i] = val;
    }
    len = n + 1;          /* now have indices 0..n stored */

    return cache[n];
}
