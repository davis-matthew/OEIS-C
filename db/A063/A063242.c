/*
 * A063242  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          92 ).
 * https://oeis.org/A063242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A063242 – Dimension of the space of weight 2n cuspidal newforms for Γ₀(92)
 * Recurrence: a(n) = a(n-2) + a(n-3) - a(n-5)   for n ≥ 7
 * Initial values: a(1)=2, a(2)=6, a(3)=8, a(4)=14, a(5)=16, a(6)=20
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Return 0 for non‑positive indices – offset starts at 1. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cached = 0;     /* highest n that has been computed */

    /* Allocate or grow cache if necessary */
    if (cached < n) {
        idx_t new_size = n;
        if (cache == NULL) {
            cache = (seq_t *)malloc((new_size + 1) * sizeof(seq_t));
            if (!cache) return (seq_t)0;          /* allocation failed */
            /* initialise first few terms */
            cache[1] = (seq_t)2;
            if (new_size >= 2) cache[2] = (seq_t)6;
            if (new_size >= 3) cache[3] = (seq_t)8;
            if (new_size >= 4) cache[4] = (seq_t)14;
            if (new_size >= 5) cache[5] = (seq_t)16;
            if (new_size >= 6) cache[6] = (seq_t)20;
            cached = (new_size >= 6 ? 6 : new_size);
        } else {
            cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
            if (!cache) return (seq_t)0;          /* reallocation failed */
            if (cached < 6) {
                /* make sure first six are initialized if we grew beyond existing size */
                cache[1] = (seq_t)2;
                if (new_size >= 2) cache[2] = (seq_t)6;
                if (new_size >= 3) cache[3] = (seq_t)8;
                if (new_size >= 4) cache[4] = (seq_t)14;
                if (new_size >= 5) cache[5] = (seq_t)16;
                if (new_size >= 6) cache[6] = (seq_t)20;
            }
        }

        /* Compute recurrence for indices > cached and at least 7 */
        idx_t start = (cached < 7 ? 7 : cached + 1);
        for (idx_t i = start; i <= n; ++i) {
            seq_t value = cache[i - 2] + cache[i - 3];
            value -= cache[i - 5];
            cache[i] = value;
        }
        cached = n;
    }

    return cache[n];
}
