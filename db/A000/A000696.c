/*
 * A000696  Numbers k such that (1,k) is "good".
 * https://oeis.org/A000696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. k must be odd. Then (1,k) is good exactly when every
 *            integer x >= 0 reaches 0 under the base-4 digit map x -> (x -
 *            d)/4, where d is taken from {0, 1, -2k, 1-2k} and d == x (mod
 *            4). Every orbit enters [0, 2k/3], so only that range is checked,
 *            using cycle detection
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   nxt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/nxt.h"

static uint64_t *mk = NULL;
static size_t mk_cap = 0;
static uint64_t mk_gen = 0;

static bool good_k(uint64_t k) {
    uint64_t k2 = 2 * k;
    uint64_t m = k2 / 3 + 1;
    if (m + 2 > mk_cap) {
        size_t nc = (size_t)(m + 2) * 2;
        mk = (uint64_t *)realloc(mk, nc * sizeof(uint64_t));
        memset(mk + mk_cap, 0, (nc - mk_cap) * sizeof(uint64_t));
        mk_cap = nc;
    }
    mk_gen++;
    uint64_t G = mk_gen << 2;
    mk[0] = G | 1;
    for (uint64_t n = 1; n <= m; n++) {
        if ((mk[n] >> 2) == mk_gen) continue;
        uint64_t x = n;
        for (;;) {
            if ((mk[x] >> 2) == mk_gen) {
                if ((mk[x] & 3) == 2) return false;
                break;
            }
            mk[x] = G | 2;
            x = nxt(x, k2);
        }
        uint64_t y = n;
        while (mk[y] == (G | 2)) {
            mk[y] = G | 1;
            y = nxt(y, k2);
        }
    }
    return true;
}

static uint64_t *terms = NULL;
static size_t nterms = 0, tcap = 0;
static uint64_t next_k = 1;

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t k = next_k;
        next_k += 2;
        if (good_k(k)) {
            if (nterms == tcap) {
                tcap = tcap ? tcap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, tcap * sizeof(uint64_t));
            }
            terms[nterms++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
