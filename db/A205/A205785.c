/*
 * A205785  Least positive integer k such that n divides C(k)-C(j) for some j
 *          in [1,k-1], where C=A205825.
 * https://oeis.org/A205785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that C(k) == C(j) (mod n) for some 1 <= j <
 *            k, where C(m) = m!/ceiling(m/2)! (A205825); found by search
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-07)
 * Helpers:   mem, mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/mulmod_seq.h"

static seq_t A205825_a(idx_t n)
{
    idx_t start = ((n + 1) / 2) + 1;
    seq_t res = 1;
    for (idx_t k = start; k <= n; ++k)
        res *= (seq_t)k;
    return res;
}

seq_t a(idx_t n)
{
    (void)A205825_a;
    if (n <= 1) return (seq_t)2;
    seq_t N = (seq_t)n;
    seq_t *r = mem_new(seq_t, (size_t)n + 3);
    seq_t ans = (seq_t)0;
    for (idx_t k = 1; k <= n + 1; ++k) {
        idx_t start = ((k + 1) / 2) + 1;
        seq_t v = (seq_t)1 % N;
        for (idx_t i = start; i <= k; ++i)
            v = mulmod_seq(v, (seq_t)i % N, N);
        r[k] = v;
        bool found = false;
        for (idx_t j = 1; j < k; ++j) {
            if (r[j] == v) { found = true; break; }
        }
        if (found) { ans = (seq_t)k; break; }
    }
    mem_free(r);
    return ans;
}
