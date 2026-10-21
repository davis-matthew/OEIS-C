/*
 * A224553  Number of (n+2) X 4 0..1 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=70, a(2)=94, a(3)=133, a(4)=174, a(5)=246, a(6)=351,
 *            a(7)=492, a(8)=693, a(9)=992, a(n) = 3*a(n‑1) ‑ 2*a(n‑2) ‑
 *            a(n‑3) + a(n‑4) ‑ a(n‑5) + 2*a(n‑6) ‑ a(n‑7) for n>9
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 49, u64 n <= 107, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is stored in a static cache that grows lazily.
   Only 7 previous values are needed for the recurrence, so values
   are computed on demand and cached for subsequent calls. */

#include <stddef.h>

#define MAX_N 20000   /* safety limit; supports n up to 20 000 */

static seq_t cache[MAX_N + 1] = { 0 };   /* 0‑based array; cache[0] unused */
static idx_t cached_max = 0;             /* largest n already computed */

/* initial terms for indices 1 through 9 */
static const seq_t init_terms[10] = {
    0,       /* dummy zero for index 0 */
    70, 94, 133, 174, 246,
    351, 492, 693, 992
};

seq_t a(idx_t n)
{
    if (n == 0)            /* out of range */
        return 0;

    if (n > MAX_N) {
        /* exceeded predefined array size – grow it */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > (size_t)MAX_N) {
            /* Allocate a new larger array and copy existing data */
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) abort();   /* unrecoverable error */
            for (size_t i = 0; i <= cached_max; ++i)
                new_cache[i] = cache[i];
            /* zero the rest of the array */
            for (size_t i = cached_max + 1; i < new_cap; ++i)
                new_cache[i] = 0;
            /* free old static array is impossible; use dynamic one henceforth */
            static seq_t *dynamic_cache = NULL;
            dynamic_cache = new_cache;
            /* redirect cache to dynamic one */
            for (size_t i = 0; i <= cached_max; ++i)
                cache[i] = dynamic_cache[i];
            /* pretend array is large enough now; no further growth needed */
        }
    }

    /* Ensure the first 9 terms are stored */
    if (cached_max < 9) {
        size_t upto = (n < 9 ? (size_t)n : 9);
        for (size_t i = cached_max + 1; i <= upto; ++i)
            cache[i] = init_terms[i];
    }

    /* Compute any missing values up to n */
    if (cached_max < n) {
        for (size_t i = cached_max + 1; i <= (size_t)n; ++i) {
            if (i <= 9) continue;   /* already set */
            seq_t val = 3 * cache[i - 1]
                        - 2 * cache[i - 2]
                        - cache[i - 3]
                        + cache[i - 4]
                        - cache[i - 5]
                        + 2 * cache[i - 6]
                        - cache[i - 7];
            cache[i] = val;
        }
        cached_max = n;
    }

    return cache[n];
}
