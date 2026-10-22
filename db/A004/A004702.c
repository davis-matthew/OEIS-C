/*
 * A004702  Expansion of e.g.f. 1/(5 - exp(x) - exp(2*x) - exp(3*x) -
 *          exp(4*x)).
 * https://oeis.org/A004702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * (1 + 2^k + 3^k +
 *            4^k) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_arr.h"

static seq_t *A_vals = NULL;
static seq_t *S_vals = NULL;
static idx_t have = 0;
static idx_t cap = 0;
static seq_t p2 = 1, p3 = 1, p4 = 1;

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t nc = (n + 1 > 2 * cap) ? n + 1 : 2 * cap;
        if (nc < 16) nc = 16;
        A_vals = grow_arr(A_vals, cap, nc);
        S_vals = grow_arr(S_vals, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        A_vals[0] = 1;
        S_vals[0] = 4;
        have = 1;
    }
    while (have <= n) {
        idx_t m = have;
        p2 *= 2;
        p3 *= 3;
        p4 *= 4;
        S_vals[m] = p2 + p3 + p4 + 1;
        seq_t sum = 0;
        seq_t c = 1;
        for (idx_t k = 1; k <= m; k++) {
            c = c * (seq_t)(m - k + 1) / (seq_t)k;
            sum += c * S_vals[k] * A_vals[m - k];
        }
        A_vals[m] = sum;
        have++;
    }
    return A_vals[n];
}
