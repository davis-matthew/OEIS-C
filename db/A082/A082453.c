/*
 * A082453  a(n)=2n+1 where n is such that A083344(n) is zero.
 * https://oeis.org/A082453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(k) = 2n+1 where n is the k-th index (n>=1) with A083344(n)=0,
 *            i.e. gcd(2n+1, lcm{1+d : d|2n+1}) = gcd(2n+1, sigma(2n+1));
 *            found by a sequential search
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

typedef unsigned __int128 u128_t;

static void zq_divs(const uint64_t *pr, const uint64_t *ex, size_t idx, size_t cnt,
                    uint64_t cur, uint64_t *lv)
{
    if (idx == cnt) {
        uint64_t x = cur + 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t v = 0;
            while (v < ex[i] && x % pr[i] == 0) { x /= pr[i]; ++v; }
            if (v > lv[i]) lv[i] = v;
        }
        return;
    }
    uint64_t pw = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        zq_divs(pr, ex, idx + 1, cnt, cur * pw, lv);
        pw *= pr[idx];
    }
}

static bool zq_is_zero(uint64_t n)
{
    uint64_t m = 2 * n + 1;
    uint64_t pr[64], ex[64], lv[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    u128_t sg = 1;
    for (size_t i = 0; i < cnt; ++i) {
        u128_t term = 1, pp = 1;
        for (uint64_t e = 0; e < ex[i]; ++e) { pp *= pr[i]; term += pp; }
        sg *= term;
        lv[i] = 0;
    }
    zq_divs(pr, ex, 0, cnt, 1, lv);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t vs = 0;
        u128_t s = sg;
        while (vs < ex[i] && s % pr[i] == 0) { s /= pr[i]; ++vs; }
        if (vs != lv[i]) return false;
    }
    return true;
}

seq_t a(idx_t k)
{
    static uint64_t last_k = 0, last_n = 0;
    uint64_t cnt = 0, n = 0;
    if (last_k != 0 && (uint64_t)k >= last_k) { cnt = last_k; n = last_n; }
    while (cnt < (uint64_t)k) {
        ++n;
        if (zq_is_zero(n)) ++cnt;
    }
    last_k = (uint64_t)k;
    last_n = n;
    return (seq_t)(2 * (seq_t)n + 1);
}
