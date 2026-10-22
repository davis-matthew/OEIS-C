/*
 * A004705  Expansion of e.g.f. 1/(8 - Sum_{k=1..7} exp(k*x)).
 * https://oeis.org/A004705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * (1^k + 2^k + ... + 7^k) *
 *            a(n-k), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *memo = NULL;   /* memo[i] = a(i), valid for i < done */
static seq_t *srow = NULL;   /* srow[k] = 1^k + ... + 7^k */
static seq_t *brow = NULL;   /* brow[k] = binomial(done-1, k) */
static seq_t pw[8];          /* pw[j] = j^(done-1) */
static idx_t cap = 0;
static idx_t done = 0;

seq_t a(idx_t n) {
    if (n + 1 > cap) {
        idx_t newcap = cap ? cap : 16;
        while (newcap < n + 1) newcap *= 2;
        memo = grow_zero(memo, cap, newcap);
        srow = grow_zero(srow, cap, newcap);
        brow = grow_zero(brow, cap, newcap);
        cap = newcap;
    }
    if (done == 0) {
        memo[0] = 1;
        brow[0] = 1;
        srow[0] = 7;
        for (int j = 1; j <= 7; j++) pw[j] = 1;
        done = 1;
    }
    while (done <= n) {
        idx_t i = done;
        /* advance binomial row from i-1 to i */
        brow[i] = 1;
        for (idx_t k = i - 1; k >= 1; k--) brow[k] += brow[k - 1];
        /* power sum for exponent i */
        seq_t s = 0;
        for (int j = 1; j <= 7; j++) {
            pw[j] *= (seq_t)j;
            s += pw[j];
        }
        srow[i] = s;
        seq_t result = 0;
        for (idx_t k = 1; k <= i; k++) {
            result += brow[k] * srow[k] * memo[i - k];
        }
        memo[i] = result;
        done++;
    }
    return memo[n];
}
