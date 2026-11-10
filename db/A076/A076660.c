/*
 * A076660  Numbers m such that A076644(m) = floor((2/3)*m*(sqrt(m)+1)).
 * https://oeis.org/A076660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search for m >= 1 with A076644(m) = floor((2/3)*m*(sqrt(m)+1)),
 *            where A076644(1)=1, A076644(m)=A076644(m-floor(sqrt(m)))+m; a(n)
 *            = n-th such m
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/mem.h"

typedef unsigned __int128 A076660_u128;

static uint64_t A076660_isqrt64(uint64_t v)
{
    uint64_t r = (uint64_t)sqrtl((long double)v);
    while (r > 0 && (A076660_u128)r * r > v) r--;
    while ((A076660_u128)(r + 1) * (r + 1) <= v) r++;
    return r;
}

static A076660_u128 A076660_isqrt128(A076660_u128 v)
{
    A076660_u128 r = (A076660_u128)sqrtl((long double)v);
    while (r > 0 && r * r > v) r--;
    while ((r + 1) * (r + 1) <= v) r++;
    return r;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0, cap = 0;
    static idx_t *found = NULL;
    static idx_t fcnt = 0, fcap = 0;
    static idx_t next_m = 1;

    if (n == 0) return 0;
    while (fcnt < n) {
        idx_t m = next_m++;
        if (m > cached) {
            if (m > cap) {
                idx_t nc = cap ? cap * 2 : 1024;
                while (nc < m) nc *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, nc);
                if (!tmp) return 0;
                cache = tmp;
                cap = nc;
            }
            for (idx_t i = cached + 1; i <= cap; ++i) {
                if (i == 1) cache[0] = (seq_t)1;
                else {
                    idx_t k = (idx_t)A076660_isqrt64((uint64_t)i);
                    cache[i - 1] = cache[i - k - 1] + (seq_t)i;
                }
            }
            cached = cap;
        }
        A076660_u128 mm = (A076660_u128)m;
        A076660_u128 s = A076660_isqrt128(4 * mm * mm * mm);
        A076660_u128 f = (2 * mm + s) / 3;
        if (cache[m - 1] == (seq_t)(uint64_t)f) {
            if (fcnt >= fcap) {
                idx_t nf = fcap ? fcap * 2 : 64;
                idx_t *t = (idx_t *)realloc(found, nf * sizeof(idx_t));
                if (!t) return 0;
                found = t;
                fcap = nf;
            }
            found[fcnt++] = m;
        }
    }
    return (seq_t)found[n - 1];
}
