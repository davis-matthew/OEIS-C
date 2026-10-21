/*
 * A203322  Numbers not of the form x^2+2*y^3+3*z^4 for nonnegative x, y, z.
 * https://oeis.org/A203322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n), the nth positive integer that is NOT
   representable as x^2 + 2*y^3 + 3*z^4 for non‑negative integers x,y,z. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static idx_t isqrt_u64(idx_t v) {
    idx_t lo = 0, hi = (v >> 1) + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= v / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Global caches to memoize results between calls. */
static bool *repr = NULL;      /* repr[i] == true if i is representable */
static idx_t repr_limit = 0;   /* largest index represented in repr */
static size_t repr_cap = 0;    /* capacity of repr array */

static seq_t *nonrep = NULL;   /* list of non‑representable numbers */
static size_t nonrep_sz = 0;   /* how many are stored */
static size_t nonrep_cap = 0;  /* capacity of nonrep array */

/* Rebuild the representable table up to 'limit' (inclusive). */
static void rebuild_repr(idx_t limit) {
    if (repr) free(repr);
    repr_cap = (size_t)limit + 1;
    repr = (bool*)malloc(repr_cap);
    if (!repr) exit(1);
    memset(repr, 0, repr_cap);

    for (idx_t y = 0; ; ++y) {
        idx_t y3 = y * y * y;
        idx_t t_y = 2 * y3;
        if (t_y > limit) break;
        for (idx_t z = 0; ; ++z) {
            idx_t z4 = z * z * z * z;
            idx_t base = t_y + 3 * z4;
            if (base > limit) break;
            idx_t rem = limit - base;
            idx_t max_x = isqrt_u64(rem);
            for (idx_t x = 0; x <= max_x; ++x) {
                idx_t val = base + x * x;
                repr[val] = 1;
            }
        }
    }
    repr_limit = limit;
}

/* Build the non‑representable list up to 'limit'. */
static void rebuild_nonrep(idx_t limit) {
    if (nonrep) free(nonrep);
    nonrep_cap = (size_t)limit;           /* upper bound on elements */
    if (nonrep_cap == 0) nonrep_cap = 1;
    nonrep = (seq_t*)malloc(nonrep_cap * sizeof(seq_t));
    if (!nonrep) exit(1);
    nonrep_sz = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        if (!repr[i]) {
            if (nonrep_sz == nonrep_cap) {
                nonrep_cap <<= 1;
                nonrep = (seq_t*)realloc(nonrep, nonrep_cap * sizeof(seq_t));
                if (!nonrep) exit(1);
            }
            nonrep[nonrep_sz++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* outside sequence range; safe guard */

    if (n <= nonrep_sz) return nonrep[n - 1];

    /* estimate a reasonable upper bound for the n‑th non‑representable number */
    idx_t limit = repr_limit;
    if (limit < n * 10) limit = n * 10;

    /* iteratively grow the limits until we have enough values */
    while (1) {
        rebuild_repr(limit);
        rebuild_nonrep(limit);
        if (nonrep_sz >= n) break;
        limit <<= 1;                    /* double the limit and retry */
    }
    return nonrep[n - 1];
}
