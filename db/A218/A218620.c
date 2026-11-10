/*
 * A218620  Least number m to appear exactly n times in A095972.
 * https://oeis.org/A218620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least m such that m occurs exactly n times in A095972
 *            (A095972(k) = k - #squares mod k); found by search, tallying
 *            A095972(k) for k up to a doubling bound L and checking m <=
 *            (L-2)/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A218620_res2(uint32_t e)
{
    if (e == 1 || e == 2) return 2;
    uint64_t pp = 2, p = 2, c = 0;
    for (uint32_t i = 3; i <= e; ++i) {
        if (i == 3) c = 3;
        else c = pp + (((uint64_t)1) << (i - 3));
        pp = p;
        p = c;
    }
    return c;
}

static uint64_t A218620_resp(uint64_t p, uint32_t e)
{
    uint64_t pp = 1, pr = (p + 1) / 2;
    if (e == 1) return pr;
    uint64_t power = p, c = 0;
    for (uint32_t i = 2; i <= e; ++i) {
        power *= p;
        uint64_t term = (power - power / p) / 2;
        c = pp + term;
        pp = pr;
        pr = c;
    }
    return c;
}

static uint64_t A218620_nonres(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t rem = n, res = 1;
    if ((rem & 1ULL) == 0) {
        uint32_t e = 0;
        while ((rem & 1ULL) == 0) { rem >>= 1; ++e; }
        res *= A218620_res2(e);
    }
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            uint32_t e = 0;
            while (rem % p == 0) { rem /= p; ++e; }
            res *= A218620_resp(p, e);
        }
    }
    if (rem > 1) res *= A218620_resp(rem, 1);
    return n - res;
}

seq_t a(idx_t n)
{
    uint64_t L = 1024;
    for (;;) {
        uint32_t *cnt = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
        if (!cnt) return (seq_t)0;
        for (uint64_t k = 1; k <= L; ++k) {
            uint64_t v = A218620_nonres(k);
            if (v <= L) cnt[v]++;
        }
        for (uint64_t m = 0; 2 * m + 2 <= L; ++m) {
            if ((uint64_t)cnt[m] == (uint64_t)n) {
                free(cnt);
                return (seq_t)m;
            }
        }
        free(cnt);
        L *= 2;
    }
}
