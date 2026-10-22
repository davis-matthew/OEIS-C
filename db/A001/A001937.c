/*
 * A001937  Expansion of (psi(x^2) / psi(-x))^3 in powers of x where psi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A001937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = f(-x^4)^3 / f(-x)^3, where f(-x)^3 = Sum_{k>=0}
 *            (-1)^k*(2k+1)*x^(k(k+1)/2) (Jacobi). So a(n) = [n = 2k(k+1)]
 *            (-1)^k*(2k+1) + Sum_{k>=1} (-1)^(k+1)*(2k+1)*a(n - k(k+1)/2),
 *            a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 53, u64 n <= 177, u128 n <= 629,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < memo_len) return memo[n];

    idx_t newlen = memo_len * 2;
    if (newlen < n + 1) newlen = n + 1;
    if (newlen < 64) newlen = 64;

    seq_t *nm = (seq_t *)malloc(newlen * sizeof(seq_t));
    for (idx_t i = 0; i < newlen; i++) nm[i] = 0;
    for (idx_t i = 0; i < memo_len; i++) nm[i] = memo[i];
    if (memo) free(memo);
    memo = nm;

    for (idx_t i = memo_len; i < newlen; i++) {
        seq_t P = 0, N = 0;
        for (idx_t k = 0;; k++) {
            idx_t t = k * (k + 1) / 2;
            if (t > i) break;
            seq_t c = (seq_t)(2 * k + 1);
            if (4 * t == i) {
                if (k % 2 == 0) P += c; else N += c;
            }
            if (k >= 1) {
                if (k % 2 == 1) P += c * memo[i - t];
                else N += c * memo[i - t];
            }
        }
        memo[i] = P - N;
    }
    memo_len = newlen;
    return memo[n];
}
