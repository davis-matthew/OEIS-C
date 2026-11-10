/*
 * A083937  A014486-encodings of the plane binary trees and plane general
 *          trees whose left(most) subtree is just a "stick": \.
 * https://oeis.org/A083937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014486(A072795(n)); A072795(n) = n + C_k where sum_{i<k}
 *            C_i <= n < sum_{i<=k} C_i (C_k Catalan); A014486(m) is the m-th
 *            Dyck word as a binary number, found by ballot-number unranking
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 47 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t a072795_idx(idx_t n)
{
    seq_t cum = 0, cat = 1;
    idx_t k = 0;
    while (1) {
        if (cum + cat > (seq_t)n)
            return (seq_t)n + cat;
        cum += cat;
        cat = cat * (seq_t)(4 * k + 2) / (seq_t)(k + 2);
        ++k;
    }
}

static seq_t a014486_unrank(seq_t m)
{
    seq_t cum = 0, cat = 1;
    long long k = 0;
    while (cum + cat <= m) {
        cum += cat;
        cat = cat * (seq_t)(4 * k + 2) / (seq_t)(k + 2);
        ++k;
    }
    seq_t r = m - cum;
    if (k == 0)
        return (seq_t)0;
    int L = (int)(2 * k);
    int S = L + 2;
    seq_t *W = mem_new(seq_t, (size_t)S * S);
    W[0] = 1;
    for (int rem = 1; rem <= L; rem++) {
        for (int bal = 0; bal <= L; bal++) {
            seq_t v = 0;
            if (bal > 0) v += W[(size_t)(rem - 1) * S + (bal - 1)];
            v += W[(size_t)(rem - 1) * S + (bal + 1)];
            W[(size_t)rem * S + bal] = v;
        }
    }
    seq_t value = 0;
    int bal = 0;
    for (int pos = 0; pos < L; pos++) {
        int rem = L - pos - 1;
        int bit = 1;
        if (bal > 0) {
            seq_t cnt = W[(size_t)rem * S + (bal - 1)];
            if (r < cnt) bit = 0;
            else r -= cnt;
        }
        if (bit) bal++; else bal--;
        value = (value << 1) | (seq_t)bit;
    }
    mem_free(W);
    return value;
}

seq_t a(idx_t n)
{
    return a014486_unrank(a072795_idx(n));
}
