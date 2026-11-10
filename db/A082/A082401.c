/*
 * A082401  a(n) = A005374(n) - floor(r*n) where r is the positive root of
 *          x^3+x-1=0.
 * https://oeis.org/A082401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005374(n) - floor(r*n), r the real root of x^3+x-1=0;
 *            A005374(n) = n - A005374(A005374(A005374(n-1))), A005374(0) = 0.
 *            floor(r*n) = largest m with m^3 + m*n^2 <= n^3 (integer binary
 *            search)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A005374_memo = NULL;
static idx_t A005374_memo_size = 0;
static idx_t A005374_done = 0;
static bool A005374_init = false;

static seq_t A005374_a(idx_t n) {
    if (!A005374_init) {
        A005374_memo_size = 1024;
        A005374_memo = mem_new(seq_t, A005374_memo_size);
        if (A005374_memo == NULL) return (seq_t)0;
        A005374_memo[0] = (seq_t)0;
        A005374_done = 0;
        A005374_init = true;
    }
    if (n == 0) return A005374_memo[0];
    if (n >= A005374_memo_size) {
        idx_t new_size = n + 1024;
        seq_t *nm = mem_new(seq_t, new_size);
        if (nm == NULL) return (seq_t)0;
        for (idx_t i = 0; i < A005374_memo_size; i++) nm[i] = A005374_memo[i];
        mem_free(A005374_memo);
        A005374_memo = nm;
        A005374_memo_size = new_size;
    }
    for (idx_t i = A005374_done + 1; i <= n; i++) {
        seq_t p1 = A005374_memo[i - 1];
        seq_t p2 = A005374_memo[(long long)p1];
        seq_t p3 = A005374_memo[(long long)p2];
        A005374_memo[i] = (seq_t)i - p3;
    }
    if (n > A005374_done) A005374_done = n;
    return A005374_memo[n];
}

static idx_t floor_r_times(idx_t n) {
    typedef unsigned __int128 u128_t;
    if (n == 0) return 0;
    u128_t n2 = (u128_t)n * n;
    u128_t n3 = n2 * n;
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        u128_t m = mid;
        u128_t v = m * m * m + m * n2;
        if (v <= n3) lo = mid; else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    seq_t h = A005374_a(n);
    seq_t f = (seq_t)floor_r_times(n);
    return h - f;
}
