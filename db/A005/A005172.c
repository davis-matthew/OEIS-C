/*
 * A005172  Number of labeled rooted trees of subsets of an n-set.
 * https://oeis.org/A005172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   A'(x) = 1 + 2*A(x) + 2*A(x)*A'(x) (from A' = (1+2A)/(1-2A)),
 *            giving a(m+1) = 2*a(m) + 2*Sum_{k=1..m}
 *            binomial(m,k)*a(k)*a(m-k+1), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 222 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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

static seq_t *av = NULL;   /* av[i] = a(i) */
static seq_t *row = NULL;  /* Pascal row (have-1) */
static idx_t have = 0;     /* largest index computed */
static idx_t cap = 0;

seq_t a(idx_t n) {
    if (n > have) {
        if (n + 2 > cap) {
            idx_t newcap = 2 * cap;
            if (newcap < n + 2) newcap = n + 2;
            if (av == NULL) {
                av = alloc_zero(newcap);
                row = alloc_zero(newcap);
            } else {
                av = grow_zero(av, cap, newcap);
                row = grow_zero(row, cap, newcap);
            }
            cap = newcap;
        }
        if (have == 0) {
            av[1] = 1;
            row[0] = 1;
            have = 1;
        }
        for (idx_t m = have; m < n; m++) {
            /* advance Pascal row from m-1 to m */
            for (idx_t k = m; k >= 1; k--) row[k] += row[k - 1];
            seq_t t = av[m];
            for (idx_t k = 1; k <= m; k++) {
                t += row[k] * av[k] * av[m - k + 1];
            }
            av[m + 1] = (seq_t)2 * t;
            have = m + 1;
        }
    }
    return av[n];
}
