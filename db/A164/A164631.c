/*
 * A164631  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   linear recurrence a(0..6) = 1, 19, 342, 6156, 110808, 1994544,
 *            35901792 a(n) = 17*(a(n‑1)+…+a(n‑6)) – 153*a(n‑7) for n ≥ 8 a(7)
 *            = 1 + 17*(a(6)+…+a(1)) – 153*a(0) (The recurrence follows from
 *            the denominator 153 t⁷ – 17 t⁶ – … – 17 t + 1 of the generating
 *            function.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[7] = {
    (seq_t)1,
    (seq_t)19,
    (seq_t)342,
    (seq_t)6156,
    (seq_t)110808,
    (seq_t)1994544,
    (seq_t)35901792
};

seq_t a(idx_t n) {
    /* --- static cache ------------------------------------ */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;   /* number of elements stored */

    /* Ensure cache large enough to hold 0..n */
    if (n >= cache_size) {
        size_t needed_cap = (size_t)n + 1;
        if (needed_cap > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 8;
            while (new_cap < needed_cap) new_cap <<= 1;

            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed */

            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise first seven terms if not already done */
        if (cache_size == 0) {
            for (idx_t i = 0; i < 7; ++i)
                cache[(size_t)i] = init[(size_t)i];
            cache_size = 7;   /* we have 0..6 stored */
        }

        /* Compute terms from cache_size up to n */
        for (idx_t k = cache_size; k <= n; ++k) {
            if (k == 7) {
                /* a(7) uses RHS = 1 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 6; ++j)
                    sum += cache[(size_t)(k - j)];
                seq_t val = (seq_t)17 * sum - (seq_t)153 * cache[0] + (seq_t)1;
                cache[(size_t)k] = val;
            } else if (k >= 8) {
                seq_t sum = 0;
                for (idx_t j = 1; j <= 6; ++j)
                    sum += cache[(size_t)(k - j)];
                seq_t val = (seq_t)17 * sum - (seq_t)153 * cache[(size_t)(k - 7)];
                cache[(size_t)k] = val;
            }
        }
        cache_size = n + 1;
    }

    return cache[(size_t)n];
}
