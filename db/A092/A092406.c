/*
 * A092406  a(1)=1, a(n) = sigma(n) if sigma(n) >= a(n-1), otherwise a(n) =
 *          a(n-1) + sigma(n).
 * https://oeis.org/A092406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) = sigma(n) if sigma(n) >= a(n-1)
 *            a(n-1)+sigma(n) otherwise where sigma(n) is the sum of divisors
 *            of n (OEIS A000203)
 * Range:     u8 n <= 17, u16 n <= 281, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array of seq_t, indices from 0..cap-1 */
    static idx_t computed = 0;       /* largest n for which cache[n] is valid */
    static size_t cap = 0;           /* allocated size of cache */

    if (n < 1) return 0;            /* sequence is defined only for n≥1 */

    /* Initialize cache on first call. */
    if (cache == NULL) {
        const size_t init_cap = 4;  /* allow indices 0..3 initially */
        cache = seq_alloc(init_cap);
        if (!cache) {               /* allocation failure */
            exit(1);
        }
        cap = init_cap;
        cache[0] = 0;               /* unused but zeroed */
        cache[1] = 1;               /* a(1)=1 */
        computed = 1;
    }

    /* Grow cache if necessary. */
    if ((size_t)n > computed) {
        /* Ensure space for indices up to n. */
        size_t new_cap = (size_t)n + 1;   /* include index n */
        if (new_cap > cap) {
            cache = seq_realloc(cache, cap, new_cap);
            if (!cache) {
                exit(1);
            }
            /* Zero the newly allocated slots (they are indeterminate). */
            for (size_t i = cap; i < new_cap; ++i) {
                cache[i] = 0;
            }
            cap = new_cap;
        }

        /* Compute missing terms. */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sigma = sigma_of(i);           /* sigma(i) */
            seq_t prev  = cache[i - 1];           /* a(i-1) */
            if (sigma >= prev)
                cache[i] = sigma;
            else
                cache[i] = prev + sigma;
        }
        computed = n;
    }

    return cache[n];
}
