/*
 * A007532  Handsome numbers: sum of positive powers of its digits; a(n) =
 *          Sum_{i=1..k} d[i]^e[i] where d[1..k] are the decimal digits of
 *          a(n), e[i] > 0.
 * https://oeis.org/A007532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   search over candidates m = 1, 2, 3, ...; keep m if m =
 *            Sum_{i=1..k} d[i]^e[i] with every e[i] > 0, where d[1..k] are
 *            the decimal digits of m. Digits 0 and 1 always add 0 and 1. The
 *            other digits are tried with backtracking, largest digit first.
 *            The exponents of equal digits never increase, a lower...
 * Range:     u8 n <= 20, u16 n <= 2611, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hs_is_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/hs_is_pow.h"

static int hs_dg[24];
static uint64_t hs_minrest[25];
static int hs_nd;

static bool hs_rec(int i, uint64_t rem, uint64_t maxp) {
    uint64_t d = (uint64_t)hs_dg[i];
    bool same = (i > 0 && hs_dg[i - 1] == hs_dg[i]);
    if (i == hs_nd - 1) {
        if (same && rem > maxp) return false;
        return hs_is_pow(rem, d);
    }
    uint64_t p = d;
    while (p + hs_minrest[i + 1] <= rem) {
        if (same && p > maxp) break;
        if (hs_rec(i + 1, rem - p, p)) return true;
        p *= d;
    }
    return false;
}

static bool hs_check(uint64_t m) {
    int cnt[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint64_t t = m;
    while (t > 0) {
        cnt[t % 10]++;
        t /= 10;
    }
    uint64_t ones = (uint64_t)cnt[1];
    if (m < ones) return false;
    uint64_t target = m - ones;
    hs_nd = 0;
    for (int d = 9; d >= 2; d--)
        for (int j = 0; j < cnt[d]; j++) hs_dg[hs_nd++] = d;
    if (hs_nd == 0) return target == 0;
    hs_minrest[hs_nd] = 0;
    for (int i = hs_nd - 1; i >= 0; i--)
        hs_minrest[i] = hs_minrest[i + 1] + (uint64_t)hs_dg[i];
    if (target < hs_minrest[0]) return false;
    return hs_rec(0, target, UINT64_MAX);
}

static uint64_t *hs_terms = NULL;
static idx_t hs_count = 0, hs_cap = 0;
static uint64_t hs_last = 0;

seq_t a(idx_t n) {
    while (hs_count < n) {
        hs_last++;
        if (hs_check(hs_last)) {
            if (hs_count == hs_cap) {
                idx_t nc = hs_cap ? hs_cap * 2 : 1024;
                hs_terms = (uint64_t *)realloc(hs_terms, (size_t)nc * sizeof(uint64_t));
                hs_cap = nc;
            }
            hs_terms[hs_count++] = hs_last;
        }
    }
    seq_t r = (seq_t)hs_terms[n - 1];
    return r;
}
