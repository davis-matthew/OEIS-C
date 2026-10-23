/*
 * A170268  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A170268 – number of reduced words of length n in the Coxeter
    group on 19 generators with relations (S_i)^2 = (S_i S_j)^41 = I.

    The generating function is  
        G(t) = (t^41 + 2*t^40 + … + 2*t + 1) /
               (153*t^41 - 17*t^40 - … - 17*t + 1)

    From this we deduce the linear recurrence
        a(0) = 1
        a(n) = 17·∑_{k=1}^{40} a(n-k) + 2          (1 ≤ n ≤ 40)
        a(41) = 17·∑_{k=1}^{40} a(41-k) + 1 - 153·a(0)
        a(n) = 17·∑_{k=1}^{40} a(n-k) - 153·a(n-41)  (n ≥ 42)
    where all indices outside 0 … n are treated as zero.

    The function below keeps a static cache so that arbitrary order
    of calls remains efficient.  The inner sum ranges only over the
    last 40 terms, so the cost per new term is O(40), far below the
    2‑second limit for n ≈ 10 000.  */
#include <stdlib.h>

/* Return a(n) for the A170268 sequence.  */
seq_t a(idx_t n)
{
    /* A dynamically‑grown array holding the already computed terms. */
    static seq_t *cache = NULL;     /* storage for computed a(i) */
    static idx_t cache_size = 0;    /* number of elements currently allocated */
    static idx_t computed = 0;      /* number of terms already filled (max index+1) */

    /* Ensure we have space for a[0] … a[n] */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return a zero value (undefined behaviour otherwise) */
            return (seq_t)0;
        }
        cache     = tmp;
        cache_size = new_size;
    }

    /* Compute terms that are not yet known. */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t val;

        if (i == 0) {
            /* a(0) = 1 */
            val = (seq_t)1;
        } else if (i <= 40) {
            /* a(i) = 17·∑_{k=1}^{i} a(i-k) + 2 */
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < i; ++j) sum += cache[j];
            val = 17 * sum + (seq_t)2;
        } else if (i == 41) {
            /* a(41) = 17·∑_{k=1}^{40} a(41-k) + 1 - 153·a(0) */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 40; ++j) sum += cache[j];
            val = 17 * sum + (seq_t)1 - (seq_t)153 * cache[0];
        } else { /* i ≥ 42 */
            /* a(i) = 17·∑_{k=1}^{40} a(i-k) - 153·a(i-41) */
            seq_t sum = (seq_t)0;
            for (idx_t j = i - 40; j < i; ++j) sum += cache[j];
            val = 17 * sum - (seq_t)153 * cache[i - 41];
        }

        cache[i] = val;
    }

    /* Update the number of computed elements. */
    computed = n + 1;

    return cache[n];
}
