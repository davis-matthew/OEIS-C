/*
 * A027408  Number of labeled servers of dimension 21.
 * https://oeis.org/A027408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(21,n)} (k-1)! * C(21,k) * C(n-1,k-1) *
 *            a(n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   comb_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/comb_seq_t.h"

seq_t a(idx_t n)
{
    /* pre‑compute constants c[k] = (k-1)! * C(21,k)   */
    static bool c_init = false;
    static seq_t c[22] = {0};

    if (!c_init) {
        c_init = true;
        seq_t fac[22];
        fac[0] = 1;
        for (idx_t i = 1; i <= 21; ++i) {
            fac[i] = fac[i - 1] * (seq_t)i;
        }
        for (idx_t k = 1; k <= 21; ++k) {
            c[k] = fac[k - 1] * comb_seq_t(21, k);
        }
    }

    /* cache for computed values  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    if (cache == NULL) {          /* first call */
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 1;
        cache_size = 1;
    }

    if (n >= cache_cap) {        /* grow cache */
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    if (n >= cache_size) {        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            idx_t kmax = (i < 21) ? i : 21;
            for (idx_t k = 1; k <= kmax; ++k) {
                seq_t bin = comb_seq_t(i - 1, k - 1);
                seq_t term = c[k] * bin;
                term = term * cache[i - k];
                sum += term;
            }
            cache[i] = sum;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
