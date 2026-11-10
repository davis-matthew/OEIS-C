/*
 * A064007  At step n in computing A064413, sequence gives smallest multiple
 *          of 3 not yet seen, divided by 3.
 * https://oeis.org/A064007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (smallest multiple of 3 not among the first n terms of
 *            the EKG sequence A064413)/3; computed by direct simulation of
 *            A064413 (a(1)=1, a(2)=2, then smallest unused number sharing a
 *            factor with the previous term)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *S_used = NULL;
static uint32_t *S_nxt = NULL;
static size_t S_cap = 0;
static uint32_t *S_res = NULL;   /* S_res[k] = answer after k terms */
static size_t S_rescap = 0;
static size_t S_cnt = 0;
static uint32_t S_last = 0;
static uint32_t S_m3 = 3;

static void A064007_ensure(size_t m)
{
    if (m < S_cap) return;
    size_t nc = S_cap ? S_cap : 1024;
    while (nc <= m) nc *= 2;
    S_used = (unsigned char *)realloc(S_used, nc);
    S_nxt = (uint32_t *)realloc(S_nxt, nc * sizeof(uint32_t));
    if (!S_used || !S_nxt) exit(1);
    memset(S_used + S_cap, 0, nc - S_cap);
    memset(S_nxt + S_cap, 0, (nc - S_cap) * sizeof(uint32_t));
    S_cap = nc;
}

static void A064007_push(uint32_t v)
{
    A064007_ensure(v);
    S_used[v] = 1;
    S_last = v;
    S_cnt++;
    A064007_ensure(S_m3);
    while (S_used[S_m3]) {
        S_m3 += 3;
        A064007_ensure(S_m3);
    }
    if (S_cnt >= S_rescap) {
        size_t nc = S_rescap ? S_rescap * 2 : 1024;
        S_res = (uint32_t *)realloc(S_res, nc * sizeof(uint32_t));
        if (!S_res) exit(1);
        S_rescap = nc;
    }
    S_res[S_cnt] = S_m3 / 3;
}

static void A064007_step(void)
{
    uint32_t x = S_last;
    uint32_t best = 0xFFFFFFFFu;
    for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; p++) {
        if ((uint64_t)p * p > x) p = x;
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            A064007_ensure(p);
            uint32_t c = S_nxt[p] ? S_nxt[p] : p;
            for (;;) {
                A064007_ensure(c);
                if (!S_used[c]) break;
                c += p;
            }
            S_nxt[p] = c;
            if (c < best) best = c;
        }
        if (x == 1) break;
    }
    A064007_push(best);
}

seq_t a(idx_t n)
{
    if (S_cnt == 0) {
        A064007_ensure(16);
        A064007_push(1);
        A064007_push(2);
    }
    while (S_cnt < (size_t)n) A064007_step();
    return (seq_t)S_res[(size_t)n];
}
