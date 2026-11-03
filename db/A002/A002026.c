/*
 * A002026  Generalized ballot numbers (first differences of Motzkin numbers).
 * https://oeis.org/A002026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   fixed-width builds use a(n) = A001006(n+1) - A001006(n) with
 *            M(k) = M(k-1) + Sum_{i=0..k-2} M(i)*M(k-2-i), M(0) = M(1) = 1
 *            (division-free). Arbitrary-precision builds use the recurrence
 *            (n+3)*a(n) = (3*n+4)*a(n-1) + (n+1)*a(n-2) - 3*(n-2)*a(n-3),
 *            a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 2052 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t fixed_compute_local(idx_t n) {
    idx_t len = n + 2;
    seq_t *M = mem_new(seq_t, (size_t)len);
    seq_t r;
    idx_t k, i;
    M[0] = 1;
    M[1] = 1;
    for (k = 2; k < len; k++) {
        seq_t s = M[k - 1];
        for (i = 0; i + 2 <= k; i++) s += M[i] * M[k - 2 - i];
        M[k] = s;
    }
    r = M[n + 1] - M[n];
    mem_free(M);
    return r;
}

static int is_fixed_width_local(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x = x * 2;
    return x == 0;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    if (is_fixed_width_local()) return fixed_compute_local(n);

    if (cache == NULL) {
        cache_len = 3;
        cache = mem_new(seq_t, (size_t)cache_len);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
    }

    if (n >= cache_len) {
        idx_t new_len = n + 256;
        seq_t *new_cache = mem_new(seq_t, (size_t)new_len);
        idx_t i;
        for (i = 0; i < cache_len; i++) new_cache[i] = cache[i];
        for (i = cache_len; i < new_len; i++) {
            seq_t c1 = (seq_t)3 * (seq_t)i + 4;
            seq_t c2 = (seq_t)i + 1;
            seq_t c3 = (seq_t)3 * ((seq_t)i - 2);
            seq_t c4 = (seq_t)i + 3;
            seq_t num = c1 * new_cache[i - 1] + c2 * new_cache[i - 2] - c3 * new_cache[i - 3];
            new_cache[i] = num / c4;
        }
        mem_free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
