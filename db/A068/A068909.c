/*
 * A068909  Number of partitions of n modulo 7.
 * https://oeis.org/A068909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) mod 7, where p(n) is the partition function. p(0) =
 *            1; for n > 0, p(n) = Σₖ (-1)^(k+1) [ p(n - k(3k-1)/2) + p(n -
 *            k(3k+1)/2) ]. The sum is over k = 1, 2, … until the arguments
 *            become negative
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Copyright: 2026-10-15; the function relies only on the standard C11/ C++17 libraries. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;          /* array of values mod 7 */
    static idx_t computed = 0;           /* largest index for which cache[i] is valid */
    static idx_t capacity = 0;           /* allocated size of cache */

    /* Allocate the cache if it has never been initialized or if it is too small */
    if (capacity == 0) {
        cache = (seq_t *)realloc(NULL, sizeof(seq_t));          /* size 1 */
        if (!cache) return 0;                                   /* allocation failed */
        cache[0] = 1;                                           /* p(0) mod 7 = 1 */
        capacity = 1;
        computed = 0;                                            /* last computed index */
    } else if (n >= capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        cache = tmp;
        /* initialise newly allocated entries to 0 */
        for (idx_t i = capacity; i < new_cap; ++i) cache[i] = 0;
        capacity = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        int sum = 0;                       /* temporary signed accumulator */
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;          /* stop when arguments become negative */
            int sign = (k & 1) ? 1 : -1;   /* (-1)^(k+1) */
            sum += sign * (int)cache[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i)
                sum += sign * (int)cache[i - pent2];
        }

        int mod = sum % 7;
        if (mod < 0) mod += 7;             /* ensure a non‑negative representative */
        cache[i] = (seq_t)mod;
    }

    computed = n;
    return cache[n];
}
