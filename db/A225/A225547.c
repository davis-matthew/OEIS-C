/*
 * A225547  Fixed points of A225546.
 * https://oeis.org/A225547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest product of a subset of A225548, the
 *            generators g(i,i) = prime(i+1)^(2^i) and g(i,j) =
 *            prime(i+1)^(2^j)*prime(j+1)^(2^i) for i<j (the Fermi-Dirac
 *            primes sit at A329050(i,j) = prime(i+1)^(2^j), and the terms are
 *            the products symmetric under transposition). Computed by
 *            generatin...
 * Range:     u8 n <= 10, u16 n <= 35, u32 n <= 164, u64 n <= 1232,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9675 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A225547_cache = NULL;
static size_t A225547_cnt = 0;

static void A225547_sift(seq_t *h, size_t start, size_t end) {
    size_t root = start;
    while (2 * root + 1 < end) {
        size_t child = 2 * root + 1;
        if (child + 1 < end && h[child] < h[child + 1]) child++;
        if (h[root] < h[child]) {
            seq_t t = h[root]; h[root] = h[child]; h[child] = t;
            root = child;
        } else break;
    }
}

static void A225547_sort(seq_t *h, size_t n) {
    if (n < 2) return;
    for (size_t s = n / 2; s > 0; s--) A225547_sift(h, s - 1, n);
    for (size_t e = n - 1; e > 0; e--) {
        seq_t t = h[0]; h[0] = h[e]; h[e] = t;
        A225547_sift(h, 0, e);
    }
}

static uint64_t A225547_prime(size_t j) {
    uint64_t c = 1;
    size_t cnt = 0;
    while (1) {
        c++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; d++)
            if (c % d == 0) { ok = false; break; }
        if (ok) { if (cnt == j) return c; cnt++; }
    }
}

/* r = p^(2^e) if <= L */
static bool A225547_pw(uint64_t p, size_t e, seq_t L, seq_t *r) {
    seq_t x = (seq_t)p;
    if (x > L) return false;
    for (size_t k = 0; k < e; k++) {
        if (x > L / x) return false;
        x = x * x;
    }
    *r = x;
    return true;
}

static bool A225547_gen(size_t i, size_t j, seq_t L, seq_t *r) {
    seq_t a, b;
    if (i == j) return A225547_pw(A225547_prime(i), i, L, r);
    if (!A225547_pw(A225547_prime(i), j, L, &a)) return false;
    if (!A225547_pw(A225547_prime(j), i, L, &b)) return false;
    if (a > L / b) return false;
    *r = a * b;
    return true;
}

static size_t A225547_build(seq_t L, seq_t **out) {
    size_t gcap = 64, gc = 0;
    seq_t *gens = mem_new(seq_t, gcap);
    for (size_t j = 0;; j++) {
        bool any = false;
        seq_t g;
        for (size_t i = 0; i <= j; i++) {
            if (!A225547_gen(i, j, L, &g)) {
                if (i == j) break;
                break;
            }
            any = true;
            if (gc == gcap) { gcap *= 2; gens = mem_resize(seq_t, gens, gcap); }
            gens[gc++] = g;
        }
        /* diagonal may be valid even if an off-diagonal failed */
        if (!any || (gc > 0 && false)) {
            if (j > 0) {
                seq_t d;
                if (A225547_gen(j, j, L, &d)) {
                    if (gc == gcap) { gcap *= 2; gens = mem_resize(seq_t, gens, gcap); }
                    gens[gc++] = d;
                    continue;
                }
                break;
            }
        } else {
            /* ensure diagonal included when loop broke before reaching i==j */
            seq_t d;
            bool have = false;
            /* check whether last pushed was the diagonal */
            if (gc > 0 && A225547_gen(j, j, L, &d)) {
                have = (gens[gc - 1] == d);
                if (!have) {
                    if (gc == gcap) { gcap *= 2; gens = mem_resize(seq_t, gens, gcap); }
                    gens[gc++] = d;
                }
            }
        }
        if (j > 0) {
            seq_t t;
            if (!A225547_gen(0, j, L, &t) && !A225547_gen(j, j, L, &t)) break;
        }
    }
    A225547_sort(gens, gc);

    size_t cap = 1024, cnt = 1;
    seq_t *p = mem_new(seq_t, cap);
    p[0] = 1;
    for (size_t gi = 0; gi < gc; gi++) {
        seq_t g = gens[gi];
        if (g > L) continue;
        seq_t lim = L / g;
        size_t c0 = cnt;
        for (size_t k = 0; k < c0; k++) {
            if (p[k] <= lim) {
                if (cnt == cap) { cap *= 2; p = mem_resize(seq_t, p, cap); }
                p[cnt++] = p[k] * g;
            }
        }
    }
    mem_free(gens);
    A225547_sort(p, cnt);
    *out = p;
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n <= A225547_cnt) return A225547_cache[(size_t)n - 1];
    seq_t L = 1;
    bool last = false;
    while (1) {
        seq_t *p = NULL;
        size_t c = A225547_build(L, &p);
        if (c >= (size_t)n) {
            if (A225547_cache) mem_free(A225547_cache);
            A225547_cache = p;
            A225547_cnt = c;
            return p[(size_t)n - 1];
        }
        mem_free(p);
        if (last) return 0;
        seq_t nl = L * 2;
        if (nl / 2 != L || nl < L) {
            nl = (L - 1) + L;
            last = true;
        }
        L = nl;
    }
}
