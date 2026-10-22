/*
 * A004913  Numbers that are the sum of at most 7 positive 11th powers.
 * https://oeis.org/A004913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: list every sum of at most 7 positive 11th powers below
 *            (B+1)^11, then sort and remove duplicates; B grows until there
 *            are at least n terms
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 2892, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t *buf = NULL;
static size_t bufcnt = 0, bufcap = 0;
static uint64_t pw[64];

static int cmp_u64(const void *x, const void *y) {
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static void push(uint64_t v) {
    if (bufcnt == bufcap) {
        bufcap = bufcap ? bufcap * 2 : 1024;
        buf = (uint64_t *)realloc(buf, bufcap * sizeof(uint64_t));
    }
    buf[bufcnt++] = v;
}

static void gen(int base, int depth, uint64_t s, int B, uint64_t limit) {
    push(s);
    if (depth >= 7) return;
    for (int b = base; b <= B; b++) {
        if (pw[b] >= limit - s) break;
        gen(b, depth + 1, s + pw[b], B, limit);
    }
}

seq_t a(idx_t n) {
    if (n > nterms) {
        for (int i = 0; i < 64; i++) {
            uint64_t p = 1;
            for (int j = 0; j < 11; j++) p *= (uint64_t)i;
            pw[i] = p;
        }
        int B = 1;
        for (;;) {
            uint64_t limit = pw[B + 1];
            bufcnt = 0;
            gen(1, 0, 0, B, limit);
            qsort(buf, bufcnt, sizeof(uint64_t), cmp_u64);
            size_t u = 0;
            for (size_t i = 0; i < bufcnt; i++)
                if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];
            if ((idx_t)u >= n || B >= 53) {
                free(terms);
                terms = (uint64_t *)malloc(u * sizeof(uint64_t));
                for (size_t i = 0; i < u; i++) terms[i] = buf[i];
                nterms = (idx_t)u;
                break;
            }
            B++;
        }
    }
    return u128_to_seq((unsigned __int128)terms[n - 1]);
}
