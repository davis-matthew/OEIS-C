/*
 * A004865  Numbers that are the sum of at most 3 positive 7th powers.
 * https://oeis.org/A004865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- list all x^7+y^7+z^7 with 0<=x<=y<=z<=B below (B+1)^7,
 *            sort, dedupe; double B until n terms exist
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 2436, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9547 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static int A004865_cmp(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static uint64_t A004865_p7(uint64_t b) {
    uint64_t r = 1;
    for (int i = 0; i < 7; i++) r *= b;
    return r;
}

static uint64_t *A004865_terms = NULL;
static idx_t A004865_count = 0;

static void A004865_build(uint64_t B) {
    uint64_t limit = A004865_p7(B + 1);
    uint64_t *pw = (uint64_t *)malloc((size_t)(B + 1) * sizeof(uint64_t));
    for (uint64_t i = 0; i <= B; i++) pw[i] = A004865_p7(i);
    size_t cap = (size_t)((B + 1) * (B + 2) / 2 * (B + 3) / 3) + 1;
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t cnt = 0;
    for (uint64_t i = 0; i <= B; i++) {
        for (uint64_t j = i; j <= B; j++) {
            uint64_t s2 = pw[i] + pw[j];
            if (s2 >= limit) break;
            for (uint64_t k = j; k <= B; k++) {
                uint64_t s = s2 + pw[k];
                if (s >= limit) break;
                buf[cnt++] = s;
            }
        }
    }
    free(pw);
    qsort(buf, cnt, sizeof(uint64_t), A004865_cmp);
    size_t u = 0;
    for (size_t i = 0; i < cnt; i++) {
        if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];
    }
    free(A004865_terms);
    A004865_terms = buf;
    A004865_count = (idx_t)u;
}

seq_t a(idx_t n) {
    if (n < 1) n = 1;
    if (n > A004865_count) {
        uint64_t B = 16;
        while (true) {
            A004865_build(B);
            if (A004865_count >= n || B >= 400) break;
            B *= 2;
            if (B > 400) B = 400;
        }
    }
    return u128_to_seq((unsigned __int128)A004865_terms[n - 1]);
}
