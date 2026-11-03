/*
 * A002099  G.f.: -1 + Product_{k>=1} (1 + prime(k)*x^prime(k)).
 * https://oeis.org/A002099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (-1 + Product_{k>=1} (1 + prime(k)*x^prime(k))),
 *            with a(0) = 0
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 117, u64 n <= 344,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 64 terms through n = 497 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *a002099_c = NULL;
static idx_t a002099_len = 0;

static void a002099_build(idx_t N) {
    idx_t len = N + 1, i;
    uint64_t p;
    if (a002099_c == NULL)
        a002099_c = alloc_zero(len);
    else
        a002099_c = grow_zero(a002099_c, a002099_len, len);
    for (i = 0; i < len; i++) a002099_c[i] = 0;
    a002099_c[0] = 1;
    for (p = 2; p <= N; p++) {
        if (!is_prime_u64(p)) continue;
        for (i = N; i >= p; i--)
            a002099_c[i] += (seq_t)p * a002099_c[i - p];
    }
    a002099_len = len;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n >= a002099_len) {
        idx_t N = n;
        if (a002099_len > 0 && 2 * (a002099_len - 1) > N) N = 2 * (a002099_len - 1);
        if (N < 64) N = 64;
        a002099_build(N);
    }
    return a002099_c[n];
}
