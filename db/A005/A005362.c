/*
 * A005362  Hoggatt sequence with parameter d=4.
 * https://oeis.org/A005362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 12*C(n,k)*C(n+1,k)*C(n+2,k)*C(n+3,k) /
 *            ((k+1)^3*(k+2)^2*(k+3)) (terms of
 *            4F3([-3-n,-2-n,-1-n,-n],[2,3,4],1)); for large n the recurrence
 *            (n+3)(n+4)(n+5)(n+6)*a(n) = 6(n+1)(n+3)(n+4)(2n+5)*a(n-1) +
 *            4(n-1)n(4n+7)(4n+9)*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   a5362_gcd, a5362_term, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"
#include "../utils/a5362_gcd.h"
#include "../utils/a5362_term.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n <= 40) {
        seq_t s = 0;
        idx_t k;
        for (k = 0; k <= n; k++) s += a5362_term(n, k);
        return s;
    }

    if (cache == NULL) {
        cache = grow_zero(NULL, 0, 2);
        cache[0] = 1;
        cache[1] = 2;
        cache_len = 2;
    }
    if (n >= cache_len) {
        idx_t newlen = n + 1;
        cache = grow_zero(cache, cache_len, newlen);
        while (cache_len < newlen) {
            idx_t m = cache_len;
            seq_t num = (seq_t)6 * (seq_t)(m + 1) * (seq_t)(m + 3) * (seq_t)(m + 4) * (seq_t)(2 * m + 5) * cache[m - 1]
                      + (seq_t)4 * (seq_t)(m - 1) * (seq_t)m * (seq_t)(4 * m + 7) * (seq_t)(4 * m + 9) * cache[m - 2];
            seq_t den = (seq_t)(m + 3) * (seq_t)(m + 4) * (seq_t)(m + 5) * (seq_t)(m + 6);
            cache[m] = num / den;
            cache_len = m + 1;
        }
    }
    return cache[n];
}
