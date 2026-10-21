/*
 * A206170  Number of 2 X (n+1) 0..3 arrays with every 2 X 2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A206170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9 + 3·2ⁿ + 2¹⁺ⁿ·3ⁿ + 2·9ⁿ) / 3 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;

/* Initial values for n = 1 … 5 */
static const seq_t init_vals[6] = {0, 15, 85, 641, 5257, 44585};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* behaviour below the offset is irrelevant */

    size_t sz = (size_t)n;                   /* safe cast – n is at most a few ten‑thousand */

    /* Allocate or grow the cache if necessary */
    if (cache_cap < sz) {
        size_t old_cap = cache_cap;
        seq_t *tmp = (seq_t *)malloc((sz + 1) * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */

        if (seq_cache) {
            /* Copy previously computed values */
            for (size_t i = 0; i <= old_cap; ++i)
                tmp[i] = seq_cache[i];
            free(seq_cache);
        }

        /* Initialise the first few terms */
        if (sz >= 5) {
            for (size_t i = 1; i <= 5; ++i)
                tmp[i] = init_vals[i];
        } else {
            /* sz < 5 – initialise only up to sz */
            for (size_t i = 1; i <= sz; ++i)
                tmp[i] = init_vals[i];
        }

        seq_cache = tmp;
        cache_cap = sz;

        /* Compute remaining terms with the recurrence:
           a(n) = 18·a(n‑1) – 101·a(n‑2) + 192·a(n‑3) – 108·a(n‑4)  for n ≥ 6 */
        /* If sz < 6, nothing to compute */
        if (cache_cap >= 6) {
            size_t start = old_cap + 1;
            if (start < 6) start = 6;
            for (size_t i = start; i <= sz; ++i) {
                seq_t val = 18 * seq_cache[i - 1];
                val += 192 * seq_cache[i - 3];
                val -= 101 * seq_cache[i - 2];
                val -= 108 * seq_cache[i - 4];
                seq_cache[i] = val;
            }
        }
    }

    return seq_cache[sz];
}
