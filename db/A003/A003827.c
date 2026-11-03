/*
 * A003827  'Core' alternating sign n X n matrices, i.e., those that are not
 *          'blown up' from a smaller matrix by inserting row i, column j with
 *          a_ij = 1 and all other entries in that row and column equal to 0.
 * https://oeis.org/A003827
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   A005130(n) = Sum_{m=0..n} binomial(n,m)^2 * (n-m)! * a(m), with
 *            a(0) = 1, a(1) = a(2) = 0, so a(n) = A005130(n) - Sum_{m=0..n-1}
 *            binomial(n,m) * n!/m! * a(m), where A005130(n) =
 *            Product_{i=0..n-1} (3i+1)!/(n+i)! is evaluated via its prime
 *            factorization
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 97 (2026-11-03)
 * Helpers:   ipow_i64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_i64.h"
#include "../utils/mem.h"

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

/* number of n X n alternating sign matrices, exact (or exact mod 2^w) */
static seq_t asm_count(idx_t m)
{
    if (m == 0) return 1;
    long long lim = 3 * (long long)m + 2;
    long long *c = (long long *)calloc((size_t)lim + 2, sizeof(long long));
    int *spf = (int *)calloc((size_t)lim + 2, sizeof(int));
    long long i, k;
    for (i = 0; i < (long long)m; i++) {
        /* (3i+1)! contributes +1 to every k in 2..3i+1 */
        c[2] += 1; c[3 * i + 2] -= 1;
        /* (m+i)! contributes -1 to every k in 2..m+i */
        c[2] -= 1; c[(long long)m + i + 1] += 1;
    }
    for (k = 1; k <= lim; k++) c[k] += c[k - 1];
    for (k = 2; k <= lim; k++)
        if (spf[k] == 0)
            for (i = k; i <= lim; i += k)
                if (spf[i] == 0) spf[i] = (int)k;
    for (k = lim; k >= 2; k--) {
        if (c[k] != 0 && spf[k] != k) {
            long long p = spf[k];
            c[p] += c[k];
            c[k / p] += c[k];
            c[k] = 0;
        }
    }
    seq_t r = 1;
    for (k = 2; k <= lim; k++)
        if (c[k] > 0) r *= ipow_i64((seq_t)k, c[k]);
    free(c);
    free(spf);
    return r;
}

static seq_t *memo = NULL;
static seq_t *row = NULL;   /* Pascal row for (cnt - 1) */
static idx_t cap = 0, cnt = 0;

seq_t a(idx_t n)
{
    while (cnt <= n) {
        idx_t m = cnt, k;
        if (cnt + 1 >= cap) {
            idx_t nc = 2 * cap + 16;
            memo = grow_zero(memo, cap, nc);
            row = grow_zero(row, cap, nc);
            cap = nc;
        }
        /* advance Pascal row to row m */
        if (m == 0) {
            row[0] = 1;
        } else {
            for (k = m; k >= 1; k--) row[k] += row[k - 1];
        }
        seq_t s = 0, fall = 1;
        for (k = 1; k <= m; k++) {
            fall *= (seq_t)(m - k + 1);
            s += row[k] * fall * memo[m - k];
        }
        memo[m] = asm_count(m) - s;
        cnt++;
    }
    return memo[n];
}
