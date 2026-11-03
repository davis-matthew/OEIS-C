/*
 * A005119  Infinitesimal generator of x*(x + 1).
 * https://oeis.org/A005119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (-1)^(i+1) * C(n-i+1,i+1) * a(n-i)
 *            * (n-2)!/(n-i-1)! for n > 1, a(1) = 1 (this is OEIS's a(n) =
 *            (n-2)!*Sum_{i=1..n-1} (-1)^(i+1)*C(n-i+1,i+1)*a(n-i)/(n-i-1)!,
 *            rewritten so that every step uses integers: (n-2)!/(n-i-1)! =
 *            (n-2)(n-3)...(n-i), and C(n-i+1,i+1) = 0 for 2i...
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 28,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 197 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
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

static seq_t *memo = NULL;
static idx_t memo_cap = 0;
static idx_t memo_done = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n >= memo_cap) {
        idx_t newcap = memo_cap ? memo_cap : 32;
        while (newcap <= n) newcap *= 2;
        memo = grow_zero(memo, memo_cap, newcap);
        memo_cap = newcap;
    }
    if (memo_done == 0) {
        memo[1] = 1;
        memo_done = 1;
    }
    for (idx_t m = memo_done + 1; m <= n; m++) {
        seq_t sum = 0;
        seq_t p = 1;
        for (idx_t i = 1; 2 * i <= m; i++) {
            if (i >= 2) p *= (seq_t)(m - i);
            seq_t term = binomial(m - i + 1, i + 1) * p * memo[m - i];
            if (i & 1) sum += term;
            else sum -= term;
        }
        memo[m] = sum;
        memo_done = m;
    }
    return memo[n];
}
