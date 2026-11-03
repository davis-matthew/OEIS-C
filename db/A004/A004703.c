/*
 * A004703  Expansion of e.g.f.
 *          1/(6-exp(x)-exp(2*x)-exp(3*x)-exp(4*x)-exp(5*x)).
 * https://oeis.org/A004703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * (1 + 2^k + 3^k +
 *            4^k + 5^k) * a(n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 172 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *zalloc_s(idx_t n)
{
    seq_t *p = mem_new(seq_t, (size_t)n);
    idx_t i;
    for (i = 0; i < n; i++) p[i] = 0;
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

static seq_t *A = NULL;
static seq_t *P = NULL;
static idx_t cap = 0;
static idx_t done = 0;
static seq_t pw2 = 1, pw3 = 1, pw4 = 1, pw5 = 1;

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t newcap = n + 1;
        if (newcap < 2 * cap) newcap = 2 * cap;
        if (cap == 0) {
            A = zalloc_s(newcap);
            P = zalloc_s(newcap);
        } else {
            A = grow_zero(A, cap, newcap);
            P = grow_zero(P, cap, newcap);
        }
        cap = newcap;
    }
    if (done == 0) {
        A[0] = 1;
        P[0] = 5;
        done = 1;
    }
    while (done <= n) {
        idx_t m = done;
        pw2 *= 2;
        pw3 *= 3;
        pw4 *= 4;
        pw5 *= 5;
        P[m] = pw2 + pw3 + pw4 + pw5 + 1;
        seq_t s = 0;
        seq_t c = 1;
        for (idx_t k = 1; k <= m; k++) {
            c = c * (seq_t)(m - k + 1) / (seq_t)k;
            s += c * P[k] * A[m - k];
        }
        A[m] = s;
        done++;
    }
    return A[n];
}
