/*
 * A068372  Numbers m such that the positive values of m - A002110(k) are all
 *          primes (k > 0).
 * https://oeis.org/A068372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: m is a term iff m - A002110(k) is prime for every k >= 1
 *            with A002110(k) < m. Candidates come from a mod-30030 wheel and
 *            a small-prime filter, and each one is checked with Miller-Rabin
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 420, u64 n <= 420,
 *            u128 n <= 420, big any n
 * Validated: standard, pass, 63 terms through n = 386 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

#define A068372_W 30030u
#define A068372_NQ 19

static uint64_t a068372_P[15];
static bool a068372_inited = false;
static uint32_t a068372_q[A068372_NQ] = {17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
static uint32_t a068372_pm[A068372_NQ][15];

static uint16_t a068372_res[16][1024];
static size_t a068372_rescnt[16];
static bool a068372_resok[16];

static uint64_t *a068372_terms = NULL;
static size_t a068372_cnt = 0;
static size_t a068372_cap = 0;
static uint64_t a068372_cur = 2;

static void a068372_init(void)
{
    static const uint32_t sp[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    uint64_t pr = 1;
    int i, t;
    if (a068372_inited)
        return;
    for (i = 0; i < 15; i++) {
        pr *= sp[i];
        a068372_P[i] = pr;
    }
    for (t = 0; t < A068372_NQ; t++)
        for (i = 0; i < 15; i++)
            a068372_pm[t][i] = (uint32_t)(a068372_P[i] % a068372_q[t]);
    a068372_inited = true;
}

static bool a068372_ok(uint64_t m)
{
    int i;
    for (i = 0; i < 15; i++) {
        if (a068372_P[i] >= m)
            break;
        if (!is_prime_u64(m - a068372_P[i]))
            return false;
    }
    return true;
}

static void a068372_build_res(int k)
{
    static const uint32_t wp[6] = {2,3,5,7,11,13};
    uint32_t r;
    size_t c = 0;
    if (a068372_resok[k])
        return;
    for (r = 0; r < A068372_W; r++) {
        bool good = true;
        int t, j;
        for (t = 0; t < 6 && good; t++) {
            uint32_t rm = r % wp[t];
            for (j = 0; j < k; j++) {
                if (rm == (uint32_t)(a068372_P[j] % wp[t])) {
                    good = false;
                    break;
                }
            }
        }
        if (good && c < 1024)
            a068372_res[k][c++] = (uint16_t)r;
    }
    a068372_rescnt[k] = c;
    a068372_resok[k] = true;
}

static bool a068372_pref(uint64_t c, int k)
{
    int t, j;
    for (t = 0; t < A068372_NQ; t++) {
        uint32_t r = (uint32_t)(c % a068372_q[t]);
        for (j = 0; j < k; j++)
            if (r == a068372_pm[t][j])
                return false;
    }
    return true;
}

static bool a068372_push(uint64_t m)
{
    if (a068372_cnt == a068372_cap) {
        size_t nc = a068372_cap ? a068372_cap * 2 : 256;
        uint64_t *np = (uint64_t *)realloc(a068372_terms, nc * sizeof(uint64_t));
        if (!np)
            return false;
        a068372_terms = np;
        a068372_cap = nc;
    }
    a068372_terms[a068372_cnt++] = m;
    return true;
}

seq_t a(idx_t n)
{
    a068372_init();
    while (a068372_cnt < (size_t)n) {
        uint64_t m = a068372_cur;
        int k;
        uint64_t lo, hi, base;
        size_t i, rc;
        if (m < 100000) {
            if (a068372_ok(m) && !a068372_push(m))
                return 0;
            a068372_cur++;
            continue;
        }
        k = 0;
        while (k < 15 && a068372_P[k] < m)
            k++;
        lo = a068372_P[k - 1];
        hi = (k < 15) ? a068372_P[k] : UINT64_MAX;
        if (m <= lo + 128) {
            if (a068372_ok(m) && !a068372_push(m))
                return 0;
            a068372_cur++;
            continue;
        }
        a068372_build_res(k);
        rc = a068372_rescnt[k];
        base = m - m % A068372_W;
        i = 0;
        while (i < rc && base + a068372_res[k][i] < m)
            i++;
        for (;;) {
            uint64_t c;
            if (i == rc) {
                i = 0;
                base += A068372_W;
            }
            c = base + a068372_res[k][i++];
            if (c > hi) {
                a068372_cur = hi + 1;
                break;
            }
            if (a068372_pref(c, k) && a068372_ok(c)) {
                if (!a068372_push(c))
                    return 0;
                if (a068372_cnt >= (size_t)n) {
                    a068372_cur = c + 1;
                    break;
                }
            }
        }
    }
    return (seq_t)a068372_terms[(size_t)n - 1];
}
