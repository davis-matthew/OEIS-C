/*
 * A006687  Duodecimal primes: p = (x^12 + y^12 )/(x^4 + y^4 ).
 * https://oeis.org/A006687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the n-th smallest prime of the form x^8 -
 *            x^4*y^4 + y^8 = (x^12 + y^12)/(x^4 + y^4) with gcd(x,y) = 1, x <
 *            y. Pairs are enumerated row by row in y. Since x^8 - x^4*y^4 +
 *            y^8 >= 3*y^8/4, every value below 3*(Y+1)^8/4 has been found
 *            once rows y <= Y are done. Primality is tested by...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static void mul256(u128 a, u128 b, u128 *hi, u128 *lo) {
    uint64_t a0 = (uint64_t)a, a1 = (uint64_t)(a >> 64);
    uint64_t b0 = (uint64_t)b, b1 = (uint64_t)(b >> 64);
    u128 p00 = (u128)a0 * b0, p01 = (u128)a0 * b1;
    u128 p10 = (u128)a1 * b0, p11 = (u128)a1 * b1;
    u128 mid = (p00 >> 64) + (uint64_t)p01 + (uint64_t)p10;
    *lo = (u128)(uint64_t)p00 | (mid << 64);
    *hi = p11 + (p01 >> 64) + (p10 >> 64) + (mid >> 64);
}

static u128 mont_mul(u128 a, u128 b, u128 m, u128 mp) {
    u128 hi, lo, uh, ul;
    mul256(a, b, &hi, &lo);
    u128 u = lo * mp;
    mul256(u, m, &uh, &ul);
    u128 r = hi + uh + (lo != 0);
    if (r >= m) r -= m;
    return r;
}

static bool isprime_u128(u128 m) {
    static const unsigned bases[13] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    int i;
    if (m < 2) return false;
    for (i = 0; i < 13; i++) {
        if (m == bases[i]) return true;
        if (m % bases[i] == 0) return false;
    }
    u128 inv = m;
    for (i = 0; i < 7; i++) inv *= (u128)2 - m * inv;
    u128 mp = (u128)0 - inv;
    u128 one = ((u128)0 - m) % m;
    u128 mone = m - one;
    u128 r2 = one;
    for (i = 0; i < 128; i++) {
        r2 += r2;
        if (r2 >= m) r2 -= m;
    }
    u128 d = m - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 13; i++) {
        u128 b = mont_mul((u128)bases[i], r2, m, mp);
        u128 x = one, e = d;
        while (e) {
            if (e & 1) x = mont_mul(x, b, m, mp);
            b = mont_mul(b, b, m, mp);
            e >>= 1;
        }
        if (x == one || x == mone) continue;
        bool ok = false;
        int j;
        for (j = 1; j < s; j++) {
            x = mont_mul(x, x, m, mp);
            if (x == mone) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static uint64_t gcd_u(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static int cmp_u128(const void *pa, const void *pb) {
    u128 a1 = *(const u128 *)pa, b1 = *(const u128 *)pb;
    return (a1 > b1) - (a1 < b1);
}

seq_t a(idx_t n) {
    static u128 *list = NULL;
    static size_t cnt = 0, cap = 0, complete = 0;
    static uint64_t Y = 0;

    while (complete < n) {
        uint64_t Ynew = (Y == 0) ? 16 : Y + Y / 4 + 1;
        uint64_t y, x;
        for (y = Y + 1; y <= Ynew; y++) {
            u128 y4 = (u128)y * y * y * y;
            u128 y8 = y4 * y4;
            for (x = 1; x < y; x++) {
                if (gcd_u(x, y) != 1) continue;
                u128 x4 = (u128)x * x * x * x;
                u128 f = y8 - x4 * y4 + x4 * x4;
                if (isprime_u128(f)) {
                    if (cnt == cap) {
                        cap = cap ? cap * 2 : 1024;
                        list = (u128 *)realloc(list, cap * sizeof(u128));
                    }
                    list[cnt++] = f;
                }
            }
        }
        qsort(list, cnt, sizeof(u128), cmp_u128);
        size_t k = 0, i;
        for (i = 0; i < cnt; i++)
            if (k == 0 || list[k - 1] != list[i]) list[k++] = list[i];
        cnt = k;
        Y = Ynew;
        u128 t = (u128)(Y + 1);
        t = t * t; t = t * t; t = t * t;
        u128 lim = 3 * t;
        complete = 0;
        while (complete < cnt && 4 * list[complete] < lim) complete++;
    }
    return u128_to_seq(list[n - 1]);
}
