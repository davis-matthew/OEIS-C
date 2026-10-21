/*
 * A094876  a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = 1; then a(n) = sum(
 *          a(2*i)*a(3*j)) where 1< = i< = n, 1< = j< = n and 2*i+3*j = n.
 * https://oeis.org/A094876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 70, u32 n <= 118, u64 n <= 220,
 *            u128 n <= 418, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094876
 *
 * Recurrence: a(1) = a(2) = … = a(6) = 1
 * For n ≥ 7,
 *   a(n) = Σ a(2*i) * a(3*j)
 *        over all i ≥ 1, j ≥ 1 such that 2*i + 3*j = n.
 *
 * The function uses a lazy dynamic programming cache so every value
 * is computed only once, irrespective of call order.
 */

#include <stdlib.h>

/* static cache: a[0] is unused; a[1] holds a(1) */
static seq_t *cache = NULL;
static idx_t  cap   = 0;     /* current cache size (cache[0..cap-1]) */

seq_t a(idx_t n)
{
    /* -------------------------------------------------------------
     * Extend the cache if needed.
     * ------------------------------------------------------------- */
    if (n >= cap) {
        idx_t old_cap = cap;
        idx_t new_cap = n + 1;                 /* need indices 1..n */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failed */
        cache = tmp;
        cap   = new_cap;

        /* Compute new entries lazily */
        idx_t start = (old_cap > 1) ? old_cap : 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = (seq_t)1;
                continue;
            }

            seq_t sum = (seq_t)0;
            idx_t half = i >> 1;               /* floor(i/2) */
            for (idx_t k = 1; k <= half; ++k) {
                idx_t rem = i - (k << 1);      /* i - 2*k */
                if (rem % 3) continue;         /* j must be integer */
                idx_t l = rem / 3;             /* j */
                if (l == 0) continue;          /* j >= 1 */

                sum += cache[k << 1] * cache[l * 3];
            }
            cache[i] = sum;
        }
    }

    /* -------------------------------------------------------------
     * At this point cache[n] is guaranteed to have a value.
     * ------------------------------------------------------------- */
    return cache[n];
}
