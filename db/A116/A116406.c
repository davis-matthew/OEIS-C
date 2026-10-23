/*
 * A116406  Expansion of ((1 + x - 2x^2) + (1+x)*sqrt(1-4x^2))/(2(1-4x^2)).
 * https://oeis.org/A116406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-1,k) Recurrence derived:
 *            if n-1 is odd, a(n) = 2*a(n-1) + binomial(n-2, (n)/2); if n-1 is
 *            even, a(n) = 2*a(n-1) - binomial(n-2, (n-1)/2); with
 *            a(0)=a(1)=1. Implemented with memoisation
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"   /* for binomial(idx_t n, idx_t k) */
#include "../utils/ensure_capacity.h" /* for ensure_capacity(size_t, size_t*, void**, size_t) */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t last_computed = 0;
    static bool init = false;

    /* initialise cache for indices 0,1 */
    if (!init) {
        ensure_capacity(2, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_computed = 1;
        init = true;
    }

    /* extend cache up to n if needed */
    if (n > last_computed) {
        ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));
        for (idx_t i = (idx_t)last_computed + 1; i <= n; ++i) {
            /* compute a(i) from a(i-1) */
            if ((i - 1) & 1) { /* i-1 is odd => n = i-1 is odd */
                seq_t b = binomial((i - 2), ((i - 1) + 1) / 2);
                cache[i] = (cache[i - 1] << 1) + b;
            } else { /* i-1 is even */
                seq_t b = binomial((i - 2), (i - 1) / 2);
                cache[i] = (cache[i - 1] << 1) - b;
            }
        }
        last_computed = n;
    }

    return cache[n];
}
