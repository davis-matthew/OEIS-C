/*
 * A165881  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=19, … and for n ≥ 10
 * a(n)=17·∑_{k=1}^{9}a(n‑k) –153·a(n‑10)+c(n),
 * where c(0)=1, c(1)…c(9)=2, c(10)=1, c(n)=0 for n>10. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* memoized terms */
static idx_t cache_size = 0;     /* number of terms currently stored */

static const seq_t coeff[11] = { (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)2,
                                 (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
                                 (seq_t)2, (seq_t)2, (seq_t)1 };

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    /* enlarge cache to accommodate index n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;

    /* compute values from current size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;

        for (idx_t k = 1; k <= 9; ++k) {
            if (i >= k)
                sum += cache[i - k] * (seq_t)17;
        }

        if (i >= 10)
            sum -= cache[i - 10] * (seq_t)153;

        if (i <= 10)
            sum += coeff[i];

        cache[i] = sum;
    }

    cache_size = new_size;
    return cache[n];
}
