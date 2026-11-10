/*
 * A087597  Triangular numbers m such that A040115(m) is also triangular.
 * https://oeis.org/A087597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search: a(n) = n-th triangular number t = k*(k+1)/2 (k =
 *            0,1,2,...) such that A040115(t) (concatenated |differences| of
 *            adjacent decimal digits) is also triangular, i.e. 8*A040115(t)+1
 *            is a perfect square
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 59, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 58 terms through n = 97 (2026-11-07)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_u128.h"

static uint32_t A087597_T4[10000];   /* diffs of a 4-digit zero-padded chunk */
static uint32_t A087597_Ttop[10000]; /* natural A040115 of c < 10000 */
static bool A087597_sq63[63], A087597_sq65[65], A087597_sq11[11], A087597_sq17[17];
static bool A087597_init = false;

static uint32_t A087597_absd(uint32_t x, uint32_t y)
{
    return x > y ? x - y : y - x;
}

static void A087597_setup(void)
{
    uint32_t c, i;
    for (c = 0; c < 10000; ++c) {
        uint32_t d3 = c / 1000, d2 = (c / 100) % 10, d1 = (c / 10) % 10, d0 = c % 10;
        A087597_T4[c] = A087597_absd(d3, d2) * 100 + A087597_absd(d2, d1) * 10
                        + A087597_absd(d1, d0);
        if (c < 10) A087597_Ttop[c] = 0;
        else if (c < 100) A087597_Ttop[c] = A087597_absd(d1, d0);
        else if (c < 1000) A087597_Ttop[c] = A087597_absd(d2, d1) * 10 + A087597_absd(d1, d0);
        else A087597_Ttop[c] = A087597_T4[c];
    }
    for (i = 0; i < 63; ++i) A087597_sq63[(i * i) % 63] = true;
    for (i = 0; i < 65; ++i) A087597_sq65[(i * i) % 65] = true;
    for (i = 0; i < 11; ++i) A087597_sq11[(i * i) % 11] = true;
    for (i = 0; i < 17; ++i) A087597_sq17[(i * i) % 17] = true;
    A087597_init = true;
}

/* A040115 of t, computed in 4-digit chunks */
static uint64_t A087597_D(uint64_t t)
{
    if (t < 10000) return A087597_Ttop[t];
    uint64_t c = t % 10000;
    t /= 10000;
    uint64_t res = A087597_T4[c];
    uint64_t mult = 1;
    for (;;) {
        uint64_t c2 = t % 10000;
        t /= 10000;
        res += (uint64_t)A087597_absd((uint32_t)(c2 % 10), (uint32_t)(c / 1000)) * 1000 * mult;
        mult *= 10000;
        if (t == 0) {
            res += (uint64_t)A087597_Ttop[c2] * mult;
            break;
        }
        res += (uint64_t)A087597_T4[c2] * mult;
        c = c2;
    }
    return res;
}

static bool A087597_is_tri(uint64_t x)
{
    if (!A087597_sq63[(8 * (x % 63) + 1) % 63]) return false;
    if (!A087597_sq65[(8 * (x % 65) + 1) % 65]) return false;
    if (!A087597_sq11[(8 * (x % 11) + 1) % 11]) return false;
    if (!A087597_sq17[(8 * (x % 17) + 1) % 17]) return false;
    unsigned __int128 s = (unsigned __int128)x * 8 + 1;
    unsigned __int128 r = isqrt_u128(s);
    return r * r == s;
}

static uint64_t *A087597_ks = NULL;
static size_t A087597_cnt = 0, A087597_cap = 0;
static uint64_t A087597_nextk = 0, A087597_nextt = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!A087597_init) A087597_setup();
    while (A087597_cnt < n) {
        uint64_t k = A087597_nextk, t = A087597_nextt;
        if (A087597_is_tri(A087597_D(t))) {
            if (A087597_cnt == A087597_cap) {
                size_t nc = A087597_cap ? A087597_cap * 2 : 256;
                uint64_t *p = (uint64_t *)realloc(A087597_ks, nc * sizeof(uint64_t));
                if (!p) return 0;
                A087597_ks = p;
                A087597_cap = nc;
            }
            A087597_ks[A087597_cnt++] = k;
        }
        A087597_nextk = k + 1;
        A087597_nextt = t + k + 1;
    }
    uint64_t k = A087597_ks[n - 1];
    return (seq_t)k * ((seq_t)k + 1) / 2;
}
