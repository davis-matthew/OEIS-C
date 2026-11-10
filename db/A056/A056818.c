/*
 * A056818  A number n is included if n^2 divides n-th term of A056855.
 * https://oeis.org/A056818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search: n is included iff Sum_{k<=n, gcd(k,n)=1} Product_{j<=n,
 *            gcd(j,n)=1, j!=k} j == 0 (mod n^2)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A056818_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* n^2 < 2^32 for all practical n here, so products of residues fit in uint64_t */
static bool A056818_ok(uint64_t n)
{
    if (n == 1) return true;
    uint64_t m = n * n;
    uint64_t *ks = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    uint64_t *pre = (uint64_t *)malloc((size_t)(n + 2) * sizeof(uint64_t));
    uint64_t *suf = (uint64_t *)malloc((size_t)(n + 2) * sizeof(uint64_t));
    size_t r = 0;
    for (uint64_t k = 1; k <= n; ++k) {
        if (A056818_gcd(k, n) == 1) ks[r++] = k;
    }
    pre[0] = 1 % m;
    for (size_t i = 0; i < r; ++i) pre[i + 1] = (pre[i] * (ks[i] % m)) % m;
    suf[r] = 1 % m;
    for (size_t i = r; i > 0; --i) suf[i - 1] = (suf[i] * (ks[i - 1] % m)) % m;
    uint64_t total = 0;
    for (size_t i = 0; i < r; ++i) {
        total += (pre[i] * suf[i + 1]) % m;
        if (total >= m) total -= m;
    }
    free(ks);
    free(pre);
    free(suf);
    return total == 0;
}

seq_t a(idx_t n)
{
    static uint64_t *found = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t cand = 0;
    while (cnt < (size_t)n) {
        ++cand;
        if (A056818_ok(cand)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 256;
                found = (uint64_t *)realloc(found, cap * sizeof(uint64_t));
            }
            found[cnt++] = cand;
        }
    }
    return (seq_t)found[n - 1];
}
