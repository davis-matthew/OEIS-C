/*
 * A098192  Terms in a specific cycle of length 29 of the map x->A098189(x).
 * https://oeis.org/A098192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   iterate x -> A098189(x) = A034448(x) - phi(x) from x=246 until
 *            it is on its cycle (length 29); a(n) = n-th smallest cycle
 *            element
 * Range:     u8 none, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static uint64_t A098192_step(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    uint64_t su = 1, ph = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pp = 1;
        for (uint64_t e = 0; e < ex[i]; ++e) pp *= pr[i];
        su *= (pp + 1);
        ph *= (pp / pr[i]) * (pr[i] - 1);
    }
    return su - ph;
}

seq_t a(idx_t n)
{
    static uint64_t cyc[64];
    static size_t len = 0;
    if (len == 0) {
        uint64_t x = 246;
        for (int i = 0; i < 2000; ++i) x = A098192_step(x);
        uint64_t s = x;
        size_t c = 0;
        do {
            cyc[c++] = x;
            x = A098192_step(x);
        } while (x != s && c < 64);
        for (size_t i = 1; i < c; ++i) {
            uint64_t v = cyc[i];
            size_t j = i;
            while (j > 0 && cyc[j - 1] > v) { cyc[j] = cyc[j - 1]; --j; }
            cyc[j] = v;
        }
        len = c;
    }
    if (n < 1 || (uint64_t)n > len) return (seq_t)0;
    return (seq_t)cyc[n - 1];
}
