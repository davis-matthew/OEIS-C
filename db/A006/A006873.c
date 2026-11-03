/*
 * A006873  Number of alternating 4-signed permutations.
 * https://oeis.org/A006873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. (sin(x)+cos(3x))/cos(4x), so a(n) = s(n) - Sum_{m>=1}
 *            (-16)^m * binomial(n,2m) * a(n-2m), where s(n) = 3^n, 1, -3^n,
 *            -1 for n == 0,1,2,3 (mod 4); a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 174 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
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
static idx_t have = 0, cap = 0;

static seq_t compute_term(idx_t k) {
    seq_t pos = 0, neg = 0;
    seq_t p3 = 1;
    idx_t i, m;
    seq_t b, p, t;

    if (k % 2 == 0)
        for (i = 0; i < k; i++) p3 *= 3;
    switch (k % 4) {
        case 0: pos += p3; break;
        case 1: pos += 1; break;
        case 2: neg += p3; break;
        default: neg += 1; break;
    }

    b = 1; /* binomial(k, 2m) */
    p = 1; /* 16^m */
    for (m = 1; 2 * m <= k; m++) {
        b = b * (seq_t)(k - 2 * m + 2);
        b = b * (seq_t)(k - 2 * m + 1);
        b = b / (seq_t)((2 * m - 1) * (2 * m));
        p = p * 16;
        t = b * p * memo[k - 2 * m];
        if (m % 2 == 1) pos += t;
        else neg += t;
    }
    return pos - neg;
}

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t newcap = cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        if (newcap < 16) newcap = 16;
        memo = grow_zero(memo, cap, newcap);
        cap = newcap;
    }
    while (have <= n) {
        memo[have] = compute_term(have);
        have++;
    }
    return memo[n];
}
