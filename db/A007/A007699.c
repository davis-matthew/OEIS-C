/*
 * A007699  Pisot sequence E(10,219): a(n) = nearest integer to a(n-1)^2 /
 *          a(n-2), starting 10, 219, ... Deviates from A007698 at 1403rd
 *          term.
 * https://oeis.org/A007699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(a(n-1)^2 / a(n-2)), a(1) = 10, a(2) = 219
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* nearest integer to p^2/q, avoiding overflow of p*p in fixed-width types */
static seq_t next_term(seq_t p, seq_t q) {
    if (q == 0) return 0;
    if (!seq_is_fixed_width()) {
        return (p * p + q / 2) / q;
    }
    seq_t t = p / q, r = p % q;
    /* p^2/q = p*t + t*r + r^2/q */
    seq_t res = p * t + t * r;
    /* compute r*r = Q*q + R with R < q, without overflow (r < q) */
    seq_t Q = 0, R = 0;
    if (r > 0) {
        seq_t m = 1;
        while (m <= r / 2) m <<= 1;
        for (; m > 0; m >>= 1) {
            Q += Q;
            if (R >= q - R) { R = R - (q - R); Q++; } else R += R;
            if (r & m) {
                if (R >= q - r) { R = R - (q - r); Q++; } else R += r;
            }
        }
    }
    res += Q;
    if (R >= q - R) res++;  /* 2R >= q: round up */
    return res;
}

seq_t a(idx_t n) {
    if (n == 1) return 10;
    if (n == 2) return 219;

    if (cache_len <= n) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *nc = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < old_len; i++) nc[i] = cache[i];
        free(cache);
        cache = nc;
        cache_len = new_len;
        if (old_len < 3) {
            cache[0] = 0;
            cache[1] = 10;
            cache[2] = 219;
            old_len = 3;
        }
        for (idx_t i = old_len; i <= n; i++) {
            cache[i] = next_term(cache[i - 1], cache[i - 2]);
        }
    }
    return cache[n];
}
