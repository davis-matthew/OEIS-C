/*
 * A005613  Cascade-realizable Boolean functions of n variables.
 * https://oeis.org/A005613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} ((-1)^(k+1) * binomial(n,k) * (2^(k+1)+1)
 *            * a(n-k)) - (-1)^n*(2^n+1)*a(1), a(0) = 2, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
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
static seq_t *row = NULL; /* row[k] = binomial(memo_len-1, k) */
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return 2;
    }

    if (memo_len == 0) {
        memo = grow_zero(memo, 0, 2);
        row = grow_zero(row, 0, 2);
        memo[0] = 2;
        memo[1] = 2;
        row[0] = 1;
        row[1] = 1;
        memo_len = 2;
    }

    if (n >= memo_len) {
        memo = grow_zero(memo, memo_len, n + 1);
        row = grow_zero(row, memo_len, n + 1);

        for (idx_t i = memo_len; i <= n; i++) {
            /* advance Pascal row from i-1 to i */
            row[i] = 1;
            for (idx_t k = i - 1; k >= 1; k--) {
                row[k] += row[k - 1];
            }

            seq_t pos = 0, neg = 0;
            seq_t p = 2; /* 2^(k+1) */
            for (idx_t k = 1; k < i; k++) {
                p *= 2;
                seq_t term = row[k] * (p + 1) * memo[i - k];
                if (k % 2 == 1) {
                    pos += term;
                } else {
                    neg += term;
                }
            }
            /* p is now 2^i (for i >= 2) */
            seq_t final_term = (p + 1) * memo[1];
            if (i % 2 == 0) {
                neg += final_term;
            } else {
                pos += final_term;
            }

            memo[i] = pos - neg;
        }
        memo_len = n + 1;
    }

    return memo[n];
}
