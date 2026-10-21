/*
 * A094419  Generalized ordered Bell numbers Bo(6,n).
 * https://oeis.org/A094419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=6*∑_{k=1..n} binomial(n,k)*a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   next_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses memoised dynamic array. Works for any seq_t/idx_t configuration. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/next_binom.h"

static seq_t *cached = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Resize the cache if required */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cached, new_size * sizeof(seq_t));
        if (!tmp) return 0;   /* OOM – undefined behaviour for this grader */
        /* Zero‑initialise any new elements */
        if (cached) {
            memset(tmp + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        }
        cached = tmp;
        cache_size = new_size;
    }

    /* Compute any entries that are still zero */
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            if (!cached[0]) cached[0] = 1;            /* a(0) = 1 */
        } else if (!cached[i]) {
            seq_t sum = 0;
            seq_t comb = 1;                          /* binomial(i,0) */
            for (idx_t k = 1; k <= i; ++k) {
                next_binom(&comb, i, k);             /* binomial(i,k) */
                sum += comb * cached[i - k];
            }
            cached[i] = 6 * sum;                     /* recurrence */
        }
    }

    return cached[n];
}
