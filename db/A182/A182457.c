/*
 * A182457  a(0)=0, a(1)=1, a(n) = (a(n-2)*a(n-1)+1) mod n.
 * https://oeis.org/A182457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = (a(n-2)*a(n-1) + 1) mod n (n ≥ 2)
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;         /* dynamic array of computed values */
    static idx_t capacity = 0;          /* allocated size in elements */
    static idx_t max_comp = (idx_t)-1;  /* highest n that has been computed */

    /* Ensure the cache has room for indices up to n */
    if (n + 1 > capacity) {
        idx_t new_cap = (n + 1) * 2;
        if (new_cap < (n + 1))          /* overflow guard, though unlikely */
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp)                      /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first two values on the first call */
    if (max_comp == (idx_t)-1) {
        cache[0] = (seq_t)0;
        /* capacity is at least 2 because (n+1)*2 ≥ 2 for all n≥0 */
        cache[1] = (seq_t)1;
        max_comp = 1;
    }

    /* Return a cached value if already computed */
    if (n <= max_comp)
        return cache[n];

    /* Compute values up to n iteratively */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t mod = (seq_t)i;                     /* mod value = i */

        /* Because of modular arithmetic: (a*b) mod i = ((a mod i)*(b mod i)) mod i */
        seq_t part1 = a1 % mod;
        seq_t part2 = a2 % mod;
        seq_t prod  = (part1 * part2) % mod;
        seq_t result = (prod + 1) % mod;

        cache[i] = result;
    }
    max_comp = n;
    return cache[n];
}
