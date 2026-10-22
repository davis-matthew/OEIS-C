/*
 * A002857  Number of Post functions of n variables.
 * https://oeis.org/A002857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000612(n)/4, where A000612(n) = (1/n!) * Sum_{partitions
 *            L of n} (n!/z_L) * 2^cyc(L), z_L = Prod_k k^(m_k) * m_k!, and
 *            cyc(L) = (1/m) * Sum_{d|m} phi(m/d) * 2^(Sum_i gcd(L_i, d)) with
 *            m = lcm(L) (cyc(L) is the number of cycles that a permutation of
 *            cycle type L induces on {0,1}^n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   a2857_gcd, a2857_is_fixed, a2857_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2857_gcd.h"
#include "../utils/a2857_is_fixed.h"
#include "../utils/a2857_phi.h"

typedef unsigned __int128 a2857_u128;

static uint32_t *a2857_num;
static size_t a2857_len;
static int a2857_parts[128];
static a2857_u128 a2857_nfact;

/* num += v * 2^pos */
static void a2857_add(a2857_u128 v, uint64_t pos)
{
    size_t idx = (size_t)(pos / 32);
    unsigned s = (unsigned)(pos % 32);
    uint32_t lim[4];
    uint64_t w[5], carry = 0;
    int i;
    for (i = 0; i < 4; i++) lim[i] = (uint32_t)(v >> (32 * i));
    for (i = 0; i < 5; i++) {
        uint64_t cur = 0;
        if (i < 4) cur = ((uint64_t)lim[i] << s) & 0xffffffffULL;
        if (i > 0) cur |= ((uint64_t)lim[i - 1] << s) >> 32;
        w[i] = cur;
    }
    for (i = 0; i < 5 || carry; i++) {
        uint64_t t = (uint64_t)a2857_num[idx + i] + (i < 5 ? w[i] : 0) + carry;
        a2857_num[idx + i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void a2857_divsmall(uint32_t d)
{
    uint64_t rem = 0;
    size_t i = a2857_len;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (rem << 32) | a2857_num[i];
        a2857_num[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void a2857_process(int k)
{
    uint64_t m = 1, sum = 0, d, cyc;
    a2857_u128 z = 1;
    int i, run = 0;
    for (i = 0; i < k; i++) {
        uint64_t p = (uint64_t)a2857_parts[i];
        m = m / a2857_gcd(m, p) * p;
        if (i > 0 && a2857_parts[i] == a2857_parts[i - 1]) run++;
        else run = 1;
        z *= (a2857_u128)p;
        z *= (a2857_u128)run;
    }
    for (d = 1; d <= m; d++) {
        if (m % d == 0) {
            uint64_t s = 0;
            for (i = 0; i < k; i++) s += a2857_gcd((uint64_t)a2857_parts[i], d);
            sum += a2857_phi(m / d) * ((uint64_t)1 << s);
        }
    }
    cyc = sum / m;
    a2857_add(a2857_nfact / z, cyc);
}

static void a2857_gen(int rem, int maxp, int k)
{
    int p;
    if (rem == 0) { a2857_process(k); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        a2857_parts[k] = p;
        a2857_gen(rem - p, p, k + 1);
    }
}

static seq_t a2857_conv(size_t lo, size_t len)
{
    seq_t r = 0;
    if (len == 1) {
        r += (idx_t)a2857_num[lo];
        return r;
    } else {
        size_t h = len / 2;
        r = a2857_conv(lo + h, len - h);
        r <<= (idx_t)(32 * h);
        r += a2857_conv(lo, h);
        return r;
    }
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    uint64_t nbits;
    idx_t k;
    if (n < 1) return r;
    nbits = (uint64_t)1 << n;
    a2857_len = (size_t)(nbits / 32 + 12);
    a2857_num = (uint32_t *)calloc(a2857_len, sizeof(uint32_t));
    a2857_nfact = 1;
    for (k = 2; k <= n; k++) a2857_nfact *= (a2857_u128)k;

    a2857_gen((int)n, (int)n, 0);

    a2857_divsmall(4);
    for (k = 2; k <= n; k++) a2857_divsmall((uint32_t)k);

    if (a2857_is_fixed()) {
        int i, b;
        for (i = 3; i >= 0; i--) {
            uint32_t limb = (size_t)i < a2857_len ? a2857_num[i] : 0;
            for (b = 3; b >= 0; b--) {
                r = r << 8;
                r += (idx_t)((limb >> (8 * b)) & 255u);
            }
        }
    } else {
        size_t len = a2857_len;
        while (len > 1 && a2857_num[len - 1] == 0) len--;
        r = a2857_conv(0, len);
    }
    free(a2857_num);
    a2857_num = NULL;
    return r;
}
