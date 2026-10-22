/*
 * A000451  Smallest number that is the sum of 3 squares in at least n ways.
 * https://oeis.org/A000451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with #{0<=x<=y<=z : x^2+y^2+z^2 = m} >= n;
 *            computed by an incremental sieve over the triples x<=y<=z
 * Range:     u8 n <= 6, u16 n <= 125, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   s451_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/s451_isqrt.h"

static uint32_t *s451_cnt = NULL;
static uint64_t s451_M = 0;
static uint64_t *s451_ans = NULL;
static uint64_t s451_maxc = 0;
static uint64_t s451_anscap = 0;

static void s451_extend(uint64_t newM) {
    uint64_t oldM = s451_M, c, b, x, m;
    s451_cnt = (uint32_t *)realloc(s451_cnt, (size_t)newM * sizeof(uint32_t));
    memset(s451_cnt + oldM, 0, (size_t)(newM - oldM) * sizeof(uint32_t));
    for (c = 0; c * c < newM; c++) {
        for (b = 0; b <= c && b * b + c * c < newM; b++) {
            uint64_t bc = b * b + c * c;
            x = 0;
            if (bc < oldM) {
                uint64_t lo = oldM - bc;
                x = s451_isqrt(lo);
                if (x * x < lo) x++;
            }
            for (; x <= b; x++) {
                uint64_t s = x * x + bc;
                if (s >= newM) break;
                s451_cnt[s]++;
            }
        }
    }
    for (m = oldM; m < newM; m++) {
        uint64_t k = s451_cnt[m];
        if (k > s451_maxc) {
            if (k + 1 > s451_anscap) {
                uint64_t cap = s451_anscap ? s451_anscap : 64;
                while (cap < k + 1) cap *= 2;
                s451_ans = (uint64_t *)realloc(s451_ans, (size_t)cap * sizeof(uint64_t));
                s451_anscap = cap;
            }
            while (s451_maxc < k) {
                s451_maxc++;
                s451_ans[s451_maxc] = m;
            }
        }
    }
    s451_M = newM;
}

seq_t a(idx_t n) {
    while (s451_maxc < (uint64_t)n) {
        uint64_t newM = s451_M < 1024 ? 1024 : 2 * s451_M;
        s451_extend(newM);
    }
    return (seq_t)s451_ans[n];
}
