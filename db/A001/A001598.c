/*
 * A001598  Number of terms in {b(1)..b(n)} relatively prime to b(n), where
 *          b(n) = A001597(n).
 * https://oeis.org/A001598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ i in 1..n : gcd(b(i), b(n)) = 1 }, b = A001597
 *            (perfect powers, b(1) = 1); perfect powers found by enumerating
 *            m^k <= bound, sorting and deduplicating
 * Range:     u8 n <= 261, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a1598_cmp, a1598_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a1598_cmp.h"
#include "../utils/a1598_g.h"

static uint64_t *a1598_pp = NULL;
static size_t a1598_cnt = 0;

/* Build the sorted, deduplicated list of all perfect powers <= B (including 1). */
static void a1598_build(uint64_t B) {
    size_t cap = 1024, len = 0;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    v[len++] = 1;
    for (uint64_t m = 2; m <= B / m; m++) {
        uint64_t p = m * m;
        for (;;) {
            if (len == cap) {
                cap *= 2;
                v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
            }
            v[len++] = p;
            if (p > B / m) break;
            p *= m;
        }
    }
    qsort(v, len, sizeof(uint64_t), a1598_cmp);
    size_t w = 0;
    for (size_t i = 0; i < len; i++) {
        if (w == 0 || v[w - 1] != v[i]) v[w++] = v[i];
    }
    if (a1598_pp) free(a1598_pp);
    a1598_pp = v;
    a1598_cnt = w;
}

static void a1598_ensure(idx_t n) {
    if (a1598_pp && a1598_cnt >= n) return;
    uint64_t B = 64;
    for (;;) {
        a1598_build(B);
        if (a1598_cnt >= n) return;
        B *= 4;
    }
}

seq_t a(idx_t n) {
    a1598_ensure(n);
    uint64_t bn = a1598_pp[n - 1];
    seq_t count = 0;
    for (idx_t i = 0; i < n; i++) {
        if (a1598_g(a1598_pp[i], bn) == 1) count++;
    }
    return count;
}
