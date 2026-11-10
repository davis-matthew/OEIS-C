/*
 * A214790  Numbers n such that A214789(n) = n-6.
 * https://oeis.org/A214790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that A214789(m) = m-6, i.e. F(m-7)+8
 *            == 0 (mod m) and no smaller k>=2 has m | F(k-1)+8. Computed by
 *            search
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A214790_fibpair(uint64_t n, uint64_t m, uint64_t *fn, uint64_t *fn1)
{
    uint64_t a = 0, b = 1 % m;
    int i;
    for (i = 63; i >= 0; --i) {
        unsigned __int128 t = (unsigned __int128)(2 * (unsigned __int128)b + m - a) % m;
        uint64_t c = (uint64_t)((unsigned __int128)a * (uint64_t)t % m);
        uint64_t d = (uint64_t)(((unsigned __int128)a * a + (unsigned __int128)b * b) % m);
        if ((n >> i) & 1) {
            a = d;
            b = (c + d) % m;
        } else {
            a = c;
            b = d;
        }
    }
    *fn = a;
    *fn1 = b;
}

static int A214790_ok(uint64_t m)
{
    uint64_t f, f1, j, p, c, r;
    if (m < 8) return 0;
    A214790_fibpair(m - 7, m, &f, &f1);
    if ((f + 8) % m != 0) return 0;
    /* verify no smaller j in [1, m-8] works */
    p = 0; c = 1 % m;
    for (j = 1; j + 7 < m; ++j) {
        if ((c + 8) % m == 0) return 0;
        r = (p + c) % m;
        p = c;
        c = r;
    }
    return 1;
}

seq_t a(idx_t n)
{
    static idx_t *cache = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 8;
    while (cnt < (size_t)n) {
        if (A214790_ok(next)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 256;
                cache = (idx_t *)realloc(cache, cap * sizeof(idx_t));
            }
            cache[cnt++] = (idx_t)next;
        }
        ++next;
    }
    return (seq_t)cache[n - 1];
}
