/*
 * A003363  Numbers that are the sum of 7 positive 6th powers.
 * https://oeis.org/A003363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search. Mark every sum of 7 positive 6th powers up to a limit L,
 *            then list the marked values in increasing order. If fewer than n
 *            values are found, raise L and repeat
 * Range:     u8 n <= 4, u16 n <= 405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9496 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void a3363_rec(int k, uint64_t minb, uint64_t sum, uint64_t L,
                      unsigned char *bits) {
    uint64_t b;
    if (k == 0) {
        bits[sum >> 3] |= (unsigned char)(1u << (sum & 7));
        return;
    }
    for (b = minb;; b++) {
        uint64_t p = b * b * b;
        p = p * p;
        if (sum + (uint64_t)k * p > L) break;
        a3363_rec(k - 1, b, sum + p, L, bits);
    }
}

static uint64_t *a3363_terms = NULL;
static idx_t a3363_cnt = 0;

static void a3363_build(uint64_t L) {
    size_t bytes = (size_t)(L / 8 + 1);
    unsigned char *bits = (unsigned char *)calloc(bytes, 1);
    uint64_t v;
    idx_t c = 0, cap = 1024;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    a3363_rec(7, 1, 0, L, bits);
    for (v = 0; v <= L; v++) {
        if (bits[v >> 3] & (1u << (v & 7))) {
            if (c == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, cap * sizeof(uint64_t));
            }
            t[c++] = v;
        }
    }
    free(bits);
    free(a3363_terms);
    a3363_terms = t;
    a3363_cnt = c;
}

seq_t a(idx_t n) {
    uint64_t L = 1u << 16;
    uint64_t v;
    seq_t r;
    while (a3363_cnt < n) {
        a3363_build(L);
        if (a3363_cnt >= n) break;
        L *= 4;
    }
    v = a3363_terms[n - 1];
    r = (seq_t)(uint32_t)(v >> 32);
    r <<= 16;
    r <<= 16;
    r += (seq_t)(uint32_t)(v & 0xFFFFFFFFu);
    return r;
}
