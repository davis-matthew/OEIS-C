/*
 * A120757  Expansion of x^2*(2+x)/(1-3*x-4*x^2-x^3).
 * https://oeis.org/A120757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3) (n >= 4) Initial values:
 *            a(1)=0, a(2)=2, a(3)=7
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 907 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Handle indices below the offset */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 7;

    static seq_t *cache = NULL;
    static idx_t cached_n_max = 0;

    /* Allocate or grow cache to accommodate index n */
    if (cache == NULL) {
        cache = mem_new(seq_t, (size_t)(n + 1));
        if (!cache) return 0;               /* allocation failure – unlikely */
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 7;
        cached_n_max = 3;
    } else if (n > cached_n_max) {
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)(n + 1));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        /* New tail entries will be computed below */
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cached_n_max + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1] + 4 * cache[i - 2] + cache[i - 3];
    }
    cached_n_max = n;

    return cache[n];
}
