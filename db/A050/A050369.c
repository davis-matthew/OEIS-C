/*
 * A050369  Number of ordered factorizations of n into 2 kinds of 2, 3 kinds
 *          of 3, ...
 * https://oeis.org/A050369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{d|n, d<n} a(d)/d (a(1) = 1)
 * Range:     u8 n <= 23, u16 n <= 239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *a_cache = NULL;
    static idx_t max_computed = 0;   /* largest n computed so far */

    /* enlarge cache if necessary */
    if (n > max_computed) {
        /* grow array to hold entries up to n (index 0 unused) */
        seq_t *tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory – return 0 as safe fallback */
        a_cache = tmp;

        /* compute new entries one by one */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                a_cache[1] = (seq_t)1;
                continue;
            }

            seq_t sum = 0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    if (d < i) {
                        sum += a_cache[d] * ((seq_t)i / (seq_t)d);
                    }
                    idx_t d2 = i / d;
                    if (d2 != d && d2 < i) {
                        sum += a_cache[d2] * ((seq_t)i / (seq_t)d2);
                    }
                }
            }
            a_cache[i] = sum;
        }

        max_computed = n;
    }

    return a_cache[n];
}
