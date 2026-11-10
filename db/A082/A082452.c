/*
 * A082452  a(n)=2n+1 where n is such that A083344(n) is not zero.
 * https://oeis.org/A082452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2k+1 where k is the n-th index with A083344(k) != 0, i.e.
 *            gcd(2k+1, lcm{d+1 : d|2k+1}) != gcd(2k+1, sigma(2k+1)); computed
 *            by a memoized scan over odd m
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A082452_nonzero(uint64_t m)
{
    uint64_t pr[24];
    int ex[24];
    int cnt = 0;
    uint64_t t = m;
    for (uint64_t d = 3; d * d <= t; d += 2) {
        if (t % d == 0) {
            pr[cnt] = d;
            ex[cnt] = 0;
            while (t % d == 0) { t /= d; ex[cnt]++; }
            cnt++;
        }
    }
    if (t > 1) { pr[cnt] = t; ex[cnt] = 1; cnt++; }

    /* divisors */
    static uint64_t divs[1 << 14];
    size_t nd = 1;
    divs[0] = 1;
    for (int i = 0; i < cnt; i++) {
        size_t cur = nd;
        uint64_t pw = 1;
        for (int e = 1; e <= ex[i]; e++) {
            pw *= pr[i];
            for (size_t j = 0; j < cur; j++) divs[nd++] = divs[j] * pw;
        }
    }

    /* sigma */
    unsigned __int128 sg = 1;
    for (int i = 0; i < cnt; i++) {
        unsigned __int128 term = 1, pp = 1;
        for (int e = 0; e < ex[i]; e++) { pp *= pr[i]; term += pp; }
        sg *= term;
    }

    for (int i = 0; i < cnt; i++) {
        uint64_t p = pr[i];
        int best = 0;
        for (size_t j = 0; j < nd && best < ex[i]; j++) {
            uint64_t x = divs[j] + 1;
            int v = 0;
            while (v < ex[i] && x % p == 0) { x /= p; v++; }
            if (v > best) best = v;
        }
        int v2 = 0;
        unsigned __int128 s = sg;
        while (v2 < ex[i] && s % p == 0) { s /= p; v2++; }
        if (best != v2) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t have = 0, cap = 0;
    static uint64_t next_m = 3;
    while (have < (size_t)n) {
        uint64_t m = next_m;
        next_m += 2;
        if (A082452_nonzero(m)) {
            if (have == cap) {
                cap = cap ? cap * 2 : 256;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[have++] = m;
        }
    }
    return (seq_t)terms[n - 1];
}
