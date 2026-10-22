/*
 * A000546  First occurrence of n consecutive numbers that take same number of
 *          steps to reach 1 in 3x+1 problem.
 * https://oeis.org/A000546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the start of the first
 *            maximal run of exactly n consecutive numbers that all take the
 *            same number of 3x+1 steps to reach 1. For n >= 2 only the pairs
 *            (i, i+1) with i == 1 (mod n-1) are tested (every run of length n
 *            contains exactly one such pair), and each equal...
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A546_CS ((uint64_t)1 << 24)
#define A546_FMAX 4096

static uint16_t *a546_cache = NULL;
static uint8_t a546_jc[1 << 16];
static uint64_t a546_jd[1 << 16];
static uint64_t a546_p3[17];
static uint64_t a546_memo[A546_FMAX];

static bool a546_init(void) {
    uint64_t k;
    int l, j;
    if (a546_cache) return true;
    a546_cache = (uint16_t *)malloc((size_t)A546_CS * sizeof(uint16_t));
    if (!a546_cache) return false;
    a546_cache[0] = 0;
    a546_cache[1] = 0;
    for (k = 2; k < A546_CS; k++) {
        if (!(k & 1)) {
            a546_cache[k] = (uint16_t)(a546_cache[k >> 1] + 1);
        } else {
            uint64_t x = k;
            unsigned s = 0;
            while (x >= k) {
                if (x & 1) { x = (3 * x + 1) >> 1; s += 2; }
                else { x >>= 1; s++; }
            }
            a546_cache[k] = (uint16_t)(s + a546_cache[x]);
        }
    }
    a546_p3[0] = 1;
    for (j = 1; j <= 16; j++) a546_p3[j] = a546_p3[j - 1] * 3;
    for (l = 0; l < (1 << 16); l++) {
        uint64_t b = (uint64_t)l;
        int c = 0;
        for (j = 0; j < 16; j++) {
            if (b & 1) { b = (3 * b + 1) >> 1; c++; }
            else b >>= 1;
        }
        a546_jc[l] = (uint8_t)c;
        a546_jd[l] = b;
    }
    return true;
}

static unsigned a546_steps(uint64_t k) {
    unsigned s = 0;
    while (k >= A546_CS) {
        unsigned l = (unsigned)(k & 0xFFFF);
        unsigned c = a546_jc[l];
        k = a546_p3[c] * (k >> 16) + a546_jd[l];
        s += 16 + c;
    }
    return s + a546_cache[k];
}

static uint64_t a546_search(uint64_t n) {
    uint64_t st = n - 1, i = 1;
    if (n == 1) {
        uint64_t k = 1;
        unsigned prev = a546_steps(1);
        uint64_t start = 1;
        for (;;) {
            unsigned s;
            k++;
            s = a546_steps(k);
            if (s != prev) {
                if (k - start == 1) return start;
                start = k;
                prev = s;
            }
        }
    }
    for (;;) {
        unsigned s = a546_steps(i);
        if (a546_steps(i + 1) == s) {
            uint64_t lo = i, hi = i + 1;
            while (lo > 1 && a546_steps(lo - 1) == s) lo--;
            while (a546_steps(hi + 1) == s) hi++;
            if (hi - lo + 1 == n) return lo;
            /* next sample >= hi */
            if (hi > i) i += ((hi - i + st - 1) / st) * st;
            else i += st;
        } else {
            i += st;
        }
    }
}

seq_t a(idx_t n) {
    if (n < 1 || n >= A546_FMAX) return 0;
    if (!a546_init()) return 0;
    if (a546_memo[n] == 0) a546_memo[n] = a546_search((uint64_t)n);
    return (seq_t)a546_memo[n];
}
