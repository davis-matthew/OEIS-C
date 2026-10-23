/*
 * A169226  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 19 * Σ_{k=1}^{27} a(n‑k) - 190 * a(n‑28) + b(n)
   where      b(0)  = 1
              b(1 …27) = 2
              b(28) = 1
              b(n≥29) = 0
   The first 28 terms are derived automatically from this recurrence. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least indices 0 … n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        if (new_size < 28) new_size = 28;                 /* need first 28 terms */

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                        /* allocation failure */

        /* Compute terms from the previous size up to new_size‑1 inclusive */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t sum = 0;

            /* Sum of the previous up to 27 terms */
            idx_t maxk = 27;
            if (i < maxk) maxk = i;
            for (idx_t k = 1; k <= maxk; ++k) {
                sum += tmp[i - k];
            }

            /* Apply the recurrence */
            sum = sum * (seq_t)19;                       /* 19 * Σ a(i‑k) */
            if (i >= 28) {
                sum -= (seq_t)190 * tmp[i - 28];          /* –190 * a(i‑28) */
            }

            /* Add the numerator coefficient */
            if (i == 0 || i == 28) {
                sum += (seq_t)1;                          /* b(i) = 1 */
            } else if (i <= 27) {
                sum += (seq_t)2;                          /* b(i) = 2 for 1…27 */
            }

            tmp[i] = sum;
        }

        cache = tmp;
        cache_size = new_size;
    }

    return cache[n];
}
