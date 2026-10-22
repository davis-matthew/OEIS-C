/*
 * A005133  Number of index n subgroups of modular group PSL_2(Z).
 * https://oeis.org/A005133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 4*a(n-3) + 2*a(n-4) + (n-3)*a(n-6) - 2*a(n-7) -
 *            (n-6)*a(n-9) + Sum_{i=1..n-7} a(n-6-i)*a(i) - Sum_{i=1..n-10}
 *            a(n-9-i)*a(i) (Godsil et al.), for n >= 10, with a(1..9) = 1, 1,
 *            4, 8, 5, 22, 42, 40, 120 computed from a(n)*(n-1)! = h(n) -
 *            Sum_{j=1..n-1} a(j)*h(n-j)*(n-1)!/(n-j)!, where h(n) =...
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 63, u128 n <= 117,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;

static void init_small(void) {
    /* t2: involutions, t3: elements of order dividing 3, on n points */
    int64_t t2[10], t3[10], h[10], fact[10], s[10];
    int k, j, m;
    t2[0] = 1; t2[1] = 1;
    t3[0] = 1; t3[1] = 1; t3[2] = 1;
    for (k = 2; k < 10; k++) t2[k] = t2[k - 1] + (int64_t)(k - 1) * t2[k - 2];
    for (k = 3; k < 10; k++) t3[k] = t3[k - 1] + (int64_t)(k - 1) * (k - 2) * t3[k - 3];
    fact[0] = 1;
    for (k = 1; k < 10; k++) fact[k] = fact[k - 1] * k;
    for (k = 0; k < 10; k++) h[k] = t2[k] * t3[k];
    s[0] = 0;
    for (k = 1; k <= 9; k++) {
        int64_t v = h[k];
        for (j = 1; j < k; j++) {
            /* f = (k-1)!/(k-j)! */
            int64_t f = 1;
            for (m = k - j + 1; m <= k - 1; m++) f *= m;
            v -= s[j] * h[k - j] * f;
        }
        s[k] = v / fact[k - 1];
    }
    cache = alloc_zero(16);
    cache_len = 16;
    for (k = 1; k <= 9; k++) cache[k] = (seq_t)(uint64_t)s[k];
    computed_up_to = 9;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (cache == NULL) init_small();
    if (n <= computed_up_to) return cache[n];

    if (n >= cache_len) {
        idx_t newlen = cache_len * 2;
        if (newlen < n + 1) newlen = n + 1;
        cache = grow_zero(cache, cache_len, newlen);
        cache_len = newlen;
    }

    for (idx_t k = computed_up_to + 1; k <= n; k++) {
        seq_t pos = 0, neg = 0;
        pos += 4 * cache[k - 3];
        pos += 2 * cache[k - 4];
        pos += (seq_t)(k - 3) * cache[k - 6];
        neg += 2 * cache[k - 7];
        neg += (seq_t)(k - 6) * cache[k - 9];
        for (idx_t i = 1; i + 7 <= k; i++) pos += cache[k - 6 - i] * cache[i];
        for (idx_t i = 1; i + 10 <= k; i++) neg += cache[k - 9 - i] * cache[i];
        cache[k] = pos - neg;
        computed_up_to = k;
    }
    return cache[n];
}
