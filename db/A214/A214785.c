/*
 * A214785  Numbers k such that k + 6 = A214783(k).
 * https://oeis.org/A214785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff the smallest m with k | Fibonacci(m-1)+3
 *            is m = k+6 (A214783(k) = k+6)
 * Range:     u8 n <= 11, u16 n <= 1312, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 59 terms through n = 4282 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* F(n) mod m by fast doubling */
static uint64_t A214785_fibmod(uint64_t n, uint64_t m)
{
    uint64_t a = 0, b = 1 % m; /* F(0), F(1) */
    int i;
    for (i = 63; i >= 0; --i) {
        uint64_t t = (2 * b + m - a) % m;
        uint64_t c = (a * t) % m;
        uint64_t d = (a * a + b * b) % m;
        if ((n >> i) & 1) {
            a = d;
            b = (c + d) % m;
        } else {
            a = c;
            b = d;
        }
    }
    return a;
}

static bool A214785_ok(uint64_t k)
{
    uint64_t m, fp, fc, nx, target;
    if ((A214785_fibmod(k + 5, k) + 3) % k != 0)
        return false;
    if (k <= 3) {
        /* small moduli: plain modular arithmetic */
        fp = 0;
        fc = 1 % k;
        for (m = 1; m < k + 6; ++m) {
            if ((fp + 3) % k == 0)
                return false;
            nx = (fp + fc) % k;
            fp = fc;
            fc = nx;
        }
        return true;
    }
    /* k > 3: F(m-1) + 3 == 0 (mod k)  <=>  F(m-1) mod k == k - 3 */
    target = k - 3;
    fp = 0;
    fc = 1;
    for (m = 1; m < k + 6; ++m) {
        if (fp == target)
            return false;
        nx = fp + fc;
        if (nx >= k)
            nx -= k;
        fp = fc;
        fc = nx;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 1;
    if (n == 0)
        return 0;
    while (cnt < (size_t)n) {
        uint64_t k = next_k++;
        if (A214785_ok(k)) {
            if (cnt == cap) {
                size_t ncap = cap ? cap * 2 : 256;
                uint64_t *p = (uint64_t *)realloc(terms, ncap * sizeof(uint64_t));
                if (!p)
                    return 0;
                terms = p;
                cap = ncap;
            }
            terms[cnt++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
