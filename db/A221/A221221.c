/*
 * A221221  Where powerbacks and powertrains coincide.
 * https://oeis.org/A221221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 0 with A133048(m) = A133500(m), found by a
 *            cached incremental search
 * Range:     u8 n <= 54, u16 n <= 7418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9168 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 u128_t221221;

static u128_t221221 A221221_pt(uint64_t m)
{
    if (m == 0) return 0;
    int dg[24];
    int d = 0;
    while (m > 0) { dg[d++] = (int)(m % 10); m /= 10; }
    u128_t221221 r = 1;
    int i;
    for (i = d - 1; i >= 1; i -= 2) {
        int b = dg[i], e = dg[i - 1];
        u128_t221221 p = 1;
        int k;
        for (k = 0; k < e; k++) p *= (u128_t221221)b;
        r *= p;
    }
    if (d % 2 == 1) r *= (u128_t221221)dg[0];
    return r;
}

static uint64_t A221221_rev(uint64_t m)
{
    uint64_t r = 0;
    while (m > 0) { r = r * 10 + m % 10; m /= 10; }
    return r;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 0;
    if (n < 1) return (seq_t)0;
    while (cnt < (size_t)n) {
        uint64_t m = next++;
        if (A221221_pt(m) == A221221_pt(A221221_rev(m))) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = m;
        }
    }
    return (seq_t)terms[n - 1];
}
