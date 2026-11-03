/*
 * A006930  Binomial transform of rooted tree numbers.
 * https://oeis.org/A006930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * A000081(k+1), where
 *            A000081(1) = 1 and A000081(m+1) = (1/m) * Sum_{k=1..m}
 *            (Sum_{d|k} d*A000081(d)) * A000081(m-k+1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 539 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
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

/* c[m] = A000081(m+1), s[k] = Sum_{d|k} d*A000081(d) */
static seq_t *c = NULL;
static seq_t *s = NULL;
static idx_t cap = 0;
static idx_t done = 0; /* c[0..done-1] and s[1..done-1] are computed */

static void ensure(idx_t max_n) {
    if (max_n < done) return;
    if (max_n >= cap) {
        idx_t new_cap = max_n + 128;
        if (c == NULL) {
            c = alloc_zero(new_cap);
            s = alloc_zero(new_cap);
        } else {
            c = grow_zero(c, cap, new_cap);
            s = grow_zero(s, cap, new_cap);
        }
        cap = new_cap;
    }
    if (done == 0) {
        c[0] = 1;
        done = 1;
    }
    for (idx_t m = done; m <= max_n; m++) {
        /* s[m] needs c[d-1] for d | m, d <= m: all available */
        seq_t sg = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                sg += (seq_t)d * c[d - 1];
                idx_t e = m / d;
                if (e != d) sg += (seq_t)e * c[e - 1];
            }
        }
        s[m] = sg;
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; k++) {
            sum += s[k] * c[m - k];
        }
        c[m] = sum / (seq_t)m;
    }
    done = max_n + 1;
}

seq_t a(idx_t n) {
    ensure(n);
    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        result += binomial(n, k) * c[k];
    }
    return result;
}
