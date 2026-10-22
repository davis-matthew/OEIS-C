/*
 * A007782  Number of factors in the infinite word formed by the Kolakoski
 *          sequence A000002.
 * https://oeis.org/A007782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of distinct length-n factors of the Kolakoski word
 *            A000002, counted by scanning a long prefix (millions of terms)
 *            with a hash set of windows
 * Range:     u8 n <= 20, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mix64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mix64.h"

#define KOL_MAX 20000000ULL

static uint8_t *kol_buf = NULL;

static void kol_init(void) {
    uint64_t i, j;
    if (kol_buf) return;
    kol_buf = (uint8_t *)malloc(KOL_MAX + 4);
    kol_buf[0] = 1; kol_buf[1] = 2; kol_buf[2] = 2;
    i = 2; j = 3;
    while (j < KOL_MAX) {
        uint8_t run = kol_buf[i];
        uint8_t val = (uint8_t)((i % 2 == 0) ? 1 : 2);
        uint8_t r;
        for (r = 0; r < run && j < KOL_MAX; r++) kol_buf[j++] = val;
        i++;
    }
    for (j = 0; j < KOL_MAX; j++) kol_buf[j] = (uint8_t)(kol_buf[j] - 1);
}

typedef struct {
    uint64_t *slots;
    uint64_t cap;
    uint64_t count;
} KeySet;

static void ks_insert_raw(KeySet *s, uint64_t key) {
    uint64_t mask = s->cap - 1;
    uint64_t p = mix64(key) & mask;
    while (s->slots[p] != 0) {
        if (s->slots[p] == key) return;
        p = (p + 1) & mask;
    }
    s->slots[p] = key;
    s->count++;
}

static void ks_grow(KeySet *s) {
    uint64_t oldcap = s->cap, k;
    uint64_t *old = s->slots;
    s->cap = oldcap * 2;
    s->slots = (uint64_t *)calloc(s->cap, sizeof(uint64_t));
    s->count = 0;
    for (k = 0; k < oldcap; k++)
        if (old[k] != 0) ks_insert_raw(s, old[k]);
    free(old);
}

static void ks_add(KeySet *s, uint64_t key) {
    if (2 * (s->count + 1) > s->cap) ks_grow(s);
    ks_insert_raw(s, key);
}

#define MP 2305843009213693951ULL /* 2^61 - 1 */

static uint64_t mulmod(uint64_t a, uint64_t b) {
    unsigned __int128 t = (unsigned __int128)a * b;
    uint64_t lo = (uint64_t)(t & MP), hi = (uint64_t)(t >> 61);
    uint64_t r = lo + hi;
    if (r >= MP) r -= MP;
    return r;
}

seq_t a(idx_t n) {
    uint64_t L, i, result;
    KeySet set;
    if (n == 0) return (seq_t)1;
    kol_init();

    L = 1000000ULL + 200000ULL * (uint64_t)n;
    if (L > KOL_MAX) L = KOL_MAX;
    if ((uint64_t)n > L) return (seq_t)0;

    set.cap = 1ULL << 12;
    set.count = 0;
    set.slots = (uint64_t *)calloc(set.cap, sizeof(uint64_t));

    if (n <= 63) {
        uint64_t wmask = (1ULL << n) - 1;
        uint64_t marker = 1ULL << n;
        uint64_t w = 0;
        for (i = 0; i < (uint64_t)n; i++) w = (w << 1) | kol_buf[i];
        ks_add(&set, w | marker);
        for (i = (uint64_t)n; i < L; i++) {
            w = ((w << 1) | kol_buf[i]) & wmask;
            ks_add(&set, w | marker);
        }
    } else {
        const uint64_t B1 = 1000003ULL, B2 = 911382323ULL;
        uint64_t h1 = 0, h2 = 0, p1 = 1, p2 = 1;
        for (i = 0; i < (uint64_t)n; i++) {
            h1 = mulmod(h1, B1) + kol_buf[i] + 1; if (h1 >= MP) h1 -= MP;
            h2 = mulmod(h2, B2) + kol_buf[i] + 1; if (h2 >= MP) h2 -= MP;
            p1 = mulmod(p1, B1);
            p2 = mulmod(p2, B2);
        }
        for (i = (uint64_t)n; ; i++) {
            uint64_t key = mix64(h1 ^ (h2 << 3) ^ (h2 >> 58)) ^ h2;
            if (key == 0) key = 1;
            ks_add(&set, key);
            if (i >= L) break;
            {
                uint64_t out1 = mulmod(p1, (uint64_t)kol_buf[i - n] + 1);
                uint64_t out2 = mulmod(p2, (uint64_t)kol_buf[i - n] + 1);
                h1 = mulmod(h1, B1) + kol_buf[i] + 1; if (h1 >= MP) h1 -= MP;
                h1 = (h1 >= out1) ? h1 - out1 : h1 + MP - out1;
                h2 = mulmod(h2, B2) + kol_buf[i] + 1; if (h2 >= MP) h2 -= MP;
                h2 = (h2 >= out2) ? h2 - out2 : h2 + MP - out2;
            }
        }
    }

    result = set.count;
    free(set.slots);
    return (seq_t)result;
}
