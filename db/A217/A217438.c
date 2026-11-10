/*
 * A217438  Triangle with the chains described in A217287.
 * https://oeis.org/A217438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n-th term of the triangle whose row m is the chain m,
 *            m+1, ..., m+A217287(m)-1 (found by search)
 * Range:     u8 n <= 1770, u16 n <= 10441, u32 n <= 10441, u64 n <= 10441,
 *            u128 n <= 10441, big any n
 * Validated: standard, pass, 100 terms through n = 9989 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static bool A217438_used_has(const uint64_t *used, size_t cnt, uint64_t p)
{
    for (size_t j = 0; j < cnt; ++j)
        if (used[j] == p) return true;
    return false;
}

/* Length of chain starting at m (A217287). */
static uint64_t A217438_chain(uint64_t m)
{
    uint64_t used[8192];
    size_t cnt = 0;
    uint64_t len = 0;
    uint64_t x = m;
    for (;;) {
        uint64_t t = x;
        uint64_t pr[64];
        size_t pc = 0;
        for (uint64_t p = 2; p * p <= t; ++p) {
            if (t % p == 0) {
                pr[pc++] = p;
                while (t % p == 0) t /= p;
            }
        }
        if (t > 1) pr[pc++] = t;
        bool has_new = false;
        for (size_t i = 0; i < pc; ++i)
            if (!A217438_used_has(used, cnt, pr[i])) { has_new = true; break; }
        if (!has_new && x != m) break;
        for (size_t i = 0; i < pc; ++i)
            if (!A217438_used_has(used, cnt, pr[i]) && cnt < 8192)
                used[cnt++] = pr[i];
        ++len;
        ++x;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t k = n;
    uint64_t row = 1;
    for (;;) {
        uint64_t len = A217438_chain(row);
        if (k <= len) return (seq_t)row + (seq_t)(k - 1);
        k -= len;
        ++row;
    }
}
