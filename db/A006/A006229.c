/*
 * A006229  Expansion of e.g.f. exp( tan x ).
 * https://oeis.org/A006229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = P_n(0), where D^n exp(tan x) = P_n(tan x) exp(tan x),
 *            P_0(t) = 1, P_{n+1}(t) = (1+t^2) * (P_n'(t) + P_n(t))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static seq_t *memo = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < memo_len) return memo[n];

    idx_t N = n;
    seq_t *c = alloc_zero(N + 2);
    c[0] = 1;

    idx_t newlen = N + 1;
    memo = grow_zero(memo, memo_len, newlen);
    memo[0] = 1;

    for (idx_t m = 0; m < N; m++) {
        /* need coefficients of P_{m+1} up to degree L = N - (m+1) */
        idx_t L = N - (m + 1);
        /* d_k = c_k + (k+1) c_{k+1}, k = 0..L */
        for (idx_t k = 0; k <= L; k++) {
            c[k] = c[k] + (seq_t)(k + 1) * c[k + 1];
        }
        /* e_k = d_k + d_{k-2} */
        for (idx_t k = L; k >= 2; k--) {
            c[k] += c[k - 2];
        }
        /* clear coefficients beyond L so they are not reused */
        c[L + 1] = 0;
        memo[m + 1] = c[0];
    }

    memo_len = newlen;
    release(c, N + 2);
    return memo[n];
}
