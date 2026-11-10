/*
 * A085183  a(n) = A053645(A057520(n)), i.e., the terms of A014486 without
 *          their most significant bit (1) and the least significant bit (0).
 * https://oeis.org/A085183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053645(A057520(n)) = A057520(n) -
 *            2^floor(log2(A057520(n))); the Dyck word is built by unranking
 *            (Catalan counting)
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* SHARED_HELPER: binomial coefficient computed multiplicatively */
static seq_t a085183_binom(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i)
        r = r * (seq_t)(N - K + i) / (seq_t)i;
    return r;
}

static seq_t a085183_catalan(idx_t m)
{
    return a085183_binom(2 * m, m) / (seq_t)(m + 1);
}

/* number of completions with rem steps left and balance bal */
static seq_t a085183_count(idx_t rem, idx_t bal)
{
    if (bal > rem) return (seq_t)0;
    if ((rem - bal) & 1) return (seq_t)0;
    idx_t t = (rem + bal) / 2;
    return a085183_binom(rem, t) - a085183_binom(rem, t + 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t target = (seq_t)n;
    seq_t sumPrev = 0;
    idx_t m = 1;
    for (;; ++m) {
        seq_t cnt = a085183_catalan(m);
        if (sumPrev + cnt >= target) break;
        sumPrev += cnt;
    }

    seq_t k = target - sumPrev - 1;

    idx_t rem = 2 * m - 1;
    idx_t bal = 1;
    seq_t val = 1;

    while (rem > 0) {
        if (bal == 0) {
            val = (val << 1) | 1;
            ++bal;
        } else {
            seq_t cnt0 = a085183_count(rem - 1, bal - 1);
            if (k < cnt0) {
                val = val << 1;
                --bal;
            } else {
                val = (val << 1) | 1;
                k -= cnt0;
                ++bal;
            }
        }
        --rem;
    }

    /* A057520 = val/2 (2m-1 bits); drop its top bit */
    seq_t half = val >> 1;
    return half - (((seq_t)1) << (2 * m - 2));
}
