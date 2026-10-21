/*
 * A059734  Carryless 11^n base 10; a(n) is carryless sum of 10*a(n-1) and
 *          a(n-1).
 * https://oeis.org/A059734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Sum[Mod[Binomial[n, m], 10]*10^m, {m, 0, n}]
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   carryless_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) is also the carryless sum of 10*a(n‑1) and a(n‑1) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/carryless_add.h"

static seq_t *cache = NULL;       /* dynamically‑grown cache of computed values */
static idx_t  cap   = 0;           /* number of elements currently cached */

/* external function required by the runner */
seq_t a(idx_t n) {
    if (n + 1 > cap) {
        /* grow the cache to size n+1 */
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;                      /* a(0) */
            } else {
                seq_t prod = cache[i - 1] * 10;    /* 10*a(n-1) */
                cache[i]   = carryless_add(prod, cache[i - 1]); /* a(n) */
            }
        }
        cap = new_cap;
    }
    return cache[n];
}
