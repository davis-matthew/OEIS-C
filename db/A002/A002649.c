/*
 * A002649  Quintan primes: p = (x^5 - y^5)/(x - y).
 * https://oeis.org/A002649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate x >= y >= 1 with f(x,y) =
 *            x^4+x^3*y+x^2*y^2+x*y^3+y^4 <= N, keep the values of f that are
 *            prime, sort them and remove duplicates; double N until there are
 *            at least n terms
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 2321, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   q_cmp, q_mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/q_cmp.h"
#include "../utils/q_mulmod.h"

static uint64_t q_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = q_mulmod(r, b, m);
        b = q_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool q_isprime(uint64_t n) {
    static const uint64_t sp[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == sp[i]) return true;
        if (n % sp[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = q_powmod(sp[i], d, n);
        if (x == 1 || x == n - 1) continue;
        int r;
        bool comp = true;
        for (r = 1; r < s; r++) {
            x = q_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static uint64_t *q_terms = NULL;
static size_t q_count = 0;

static void q_build(uint64_t N) {
    size_t cap = 1024, cnt = 0;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t y, x;
    for (y = 1; 5 * y * y * y * y <= N; y++) {
        uint64_t y2 = y * y, y3 = y2 * y, y4 = y2 * y2;
        for (x = y;; x++) {
            uint64_t x2 = x * x;
            uint64_t f = x2 * x2 + x2 * x * y + x2 * y2 + x * y3 + y4;
            if (f > N) break;
            if (q_isprime(f)) {
                if (cnt == cap) {
                    cap *= 2;
                    v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
                }
                v[cnt++] = f;
            }
        }
    }
    qsort(v, cnt, sizeof(uint64_t), q_cmp);
    size_t k = 0, i;
    for (i = 0; i < cnt; i++)
        if (k == 0 || v[k - 1] != v[i]) v[k++] = v[i];
    free(q_terms);
    q_terms = v;
    q_count = k;
}

seq_t a(idx_t n) {
    static uint64_t bound = 0;
    if (n == 0) return 0;
    if (bound == 0) {
        bound = 100000;
        q_build(bound);
    }
    while (q_count < n) {
        bound *= 4;
        q_build(bound);
    }
    return (seq_t)q_terms[n - 1];
}
