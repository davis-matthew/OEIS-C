/*
 * A006755  The generalized Conway-Guy sequence w^{1}.
 * https://oeis.org/A006755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n+1) = 2*a(n) - a(n - r), r = round(sqrt(2*(n-1))), for n >=
 *            7, with a(0..7) = 0, 2, 3, 4, 8, 14, 25, 47
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

/* round(sqrt(x)) for x >= 1: the largest k with k*(k-1) < x */
static idx_t round_sqrt(idx_t x) {
    idx_t k = 1;
    while ((k + 1) * k < x) k++;
    return k;
}

seq_t a(idx_t n) {
    static const int init[8] = {0, 2, 3, 4, 8, 14, 25, 47};
    if (n < 8) return (seq_t)init[n];

    seq_t *v = alloc_zero(n + 1);
    for (idx_t i = 0; i < 8; i++) v[i] = (seq_t)init[i];
    for (idx_t m = 7; m < n; m++) {
        idx_t r = round_sqrt(2 * (m - 1));
        v[m + 1] = v[m] + v[m] - v[m - r];
    }
    seq_t res = v[n];
    release(v, n + 1);
    return res;
}
