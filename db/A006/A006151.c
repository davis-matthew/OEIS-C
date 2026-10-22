/*
 * A006151  Number of 5-tuples (p_1, p_2, ..., p_5) of Dyck paths of
 *          semilength n, such that each p_i is never below p_{i-1}.
 * https://oeis.org/A006151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            32*(2n-1)(2n+1)(2n+3)(2n+5)(2n+7)/((n+5)(n+6)(n+7)(n+8)(n+9)) *
 *            a(n-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   g64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/g64.h"

static seq_t *memo = NULL;
static idx_t memo_cap = 0;
static idx_t memo_computed = 0;

seq_t a(idx_t n) {
    if (memo == NULL) {
        memo_cap = 64;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0] = 1;
        memo_computed = 1;
    }
    if (n < memo_computed) return memo[n];

    if (n >= memo_cap) {
        idx_t newcap = memo_cap;
        while (n >= newcap) newcap *= 2;
        seq_t *nm = (seq_t *)malloc(newcap * sizeof(seq_t));
        for (idx_t k = 0; k < memo_computed; k++) nm[k] = memo[k];
        free(memo);
        memo = nm;
        memo_cap = newcap;
    }

    for (idx_t i = memo_computed; i <= n; i++) {
        uint64_t num[6], den[5];
        int p, q;
        num[0] = 32;
        num[1] = 2 * i - 1;
        num[2] = 2 * i + 1;
        num[3] = 2 * i + 3;
        num[4] = 2 * i + 5;
        num[5] = 2 * i + 7;
        for (q = 0; q < 5; q++) den[q] = i + 5 + (uint64_t)q;

        /* cancel common factors between small numerator and denominator terms */
        for (q = 0; q < 5; q++) {
            for (p = 0; p < 6; p++) {
                uint64_t g = g64(num[p], den[q]);
                if (g > 1) {
                    num[p] /= g;
                    den[q] /= g;
                }
            }
        }

        /* remaining denominators are coprime to the numerator, so divide a(i-1) exactly */
        seq_t v = memo[i - 1];
        for (q = 0; q < 5; q++) {
            if (den[q] > 1) v /= (seq_t)den[q];
        }
        for (p = 0; p < 6; p++) {
            if (num[p] > 1) v *= (seq_t)num[p];
        }
        memo[i] = v;
    }

    memo_computed = n + 1;
    return memo[n];
}
