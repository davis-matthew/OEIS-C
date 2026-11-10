/*
 * A226462  Smallest number m such that A226460(m) = n.
 * https://oeis.org/A226462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least m with A226460(m) = n. Computed by a sieve: for
 *            each prime p, the exponents e_p(k!) skipped at multiples k of
 *            p^2 are tallied up to a limit L, doubling L until the first m
 *            with count n is found
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A226462_leg(uint64_t m, uint64_t p)
{
    uint64_t s = 0;
    while (m) { m /= p; s += m; }
    return s;
}

static bool A226462_search(uint64_t n, uint64_t L, uint64_t *res)
{
    uint16_t *cnt = (uint16_t*)calloc((size_t)L + 1, sizeof(uint16_t));
    bool *comp = (bool*)calloc((size_t)L + 1, sizeof(bool));
    if (!cnt || !comp) exit(1);
    for (uint64_t p = 2; p <= L; ++p) {
        if (comp[p]) continue;
        for (uint64_t j = p * p; j <= L; j += p) comp[j] = true;
        uint64_t p2 = p * p;
        for (uint64_t t = 1; ; ++t) {
            uint64_t k = t * p2;
            uint64_t ep = A226462_leg(k - 1, p);
            if (ep >= L) break;
            uint64_t v = 2;
            uint64_t tt = t;
            while (tt % p == 0) { tt /= p; ++v; }
            for (uint64_t e = ep + 1; e < ep + v && e <= L; ++e)
                if (cnt[e] < 65535) cnt[e]++;
        }
    }
    bool found = false;
    for (uint64_t m = 0; m <= L; ++m) {
        if (cnt[m] == n) { *res = m; found = true; break; }
    }
    free(cnt);
    free(comp);
    return found;
}

seq_t a(idx_t n)
{
    static uint64_t memo[256];
    static bool have[256];
    if (n == 0) return (seq_t)0;
    if (n < 256 && have[n]) return (seq_t)memo[n];
    uint64_t res = 0;
    for (uint64_t L = (uint64_t)1 << 17; L <= ((uint64_t)1 << 30); L <<= 1) {
        if (A226462_search((uint64_t)n, L, &res)) break;
    }
    if (n < 256) { memo[n] = res; have[n] = true; }
    return (seq_t)res;
}
