/*
 * A007099  Number of labeled trivalent (or cubic) 2-connected graphs with 2n
 *          nodes.
 * https://oeis.org/A007099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * (s(n) - 2*s(n-1)) / (3*n*2^n), where s(1) = 0,
 *            s(2) = 1, s(n) = 3*n*s(n-1) + 2*s(n-2) + (3*n-1) *
 *            Sum_{i=2..n-3} s(i)*s(n-1-i). Computed as a(n) = (n-1)! *
 *            (5*7*...*(2n-1)) * (s(n) - 2*s(n-1)), since (2n)!/(3*n*2^n) =
 *            (n-1)! * (2n-1)!!/3, so no division is needed
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *S = NULL;
static idx_t S_len = 0; /* entries 0..S_len-1 valid */

static void ensure_s(idx_t n) {
    if (n < S_len) return;
    idx_t newlen = n + 1;
    if (newlen < 2 * S_len) newlen = 2 * S_len;
    if (newlen < 3) newlen = 3;
    idx_t old = S_len;
    S = grow_zero(S, S_len, newlen);
    if (old == 0) {
        S[0] = 0;
        S[1] = 0;
        S[2] = 1;
        old = 3;
    } else if (old < 3) {
        S[1] = 0;
        S[2] = 1;
        old = 3;
    }
    for (idx_t i = old; i < newlen; i++) {
        seq_t sum = 0;
        for (idx_t j = 2; j + 3 <= i; j++) {
            sum += S[j] * S[i - 1 - j];
        }
        S[i] = 3 * (seq_t)i * S[i - 1] + 2 * S[i - 2] + (3 * (seq_t)i - 1) * sum;
    }
    S_len = newlen;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    ensure_s(n);
    seq_t diff = S[n] - 2 * S[n - 1];
    seq_t c = 1;
    for (idx_t i = 2; i < n; i++) c *= (seq_t)i;
    for (idx_t k = 5; k <= 2 * n - 1; k += 2) c *= (seq_t)k;
    return c * diff;
}
