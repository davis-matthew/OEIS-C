/*
 * A096973  a(1)= 10000, a(2)= 10000; for n>2, a(n)= ( a(n-2) + a(n-1) ) (mod
 *          20000).
 * https://oeis.org/A096973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=10000, a(2)=10000, a(3)=2; a(n) = (a(n-2)+a(n-1)) mod 20000
 *            for n > 3 Computation is memoized so repeated calls are O(1)
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)10000;
    if (n == (idx_t)2) return (seq_t)10000;
    if (n == (idx_t)3) return (seq_t)2;

    /* Static state for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_comp = 2;   /* highest index computed (0‑based) */

    /* Allocate/extend cache if necessary */
    if (!cache) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)10000;
        cache[1] = (seq_t)10000;
        cache[2] = (seq_t)2;
    }
    if ((idx_t)cap < n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_comp + 1; i < n; ++i) {
        cache[i] = (cache[i-2] + cache[i-1]) % (seq_t)20000;
    }
    max_comp = (idx_t)(n-1);

    return cache[n-1];
}
