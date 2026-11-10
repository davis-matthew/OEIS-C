/*
 * A076065  Final members of groups in A076063.
 * https://oeis.org/A076065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = last element of the n-th group in A076063 (group n has n
 *            numbers, sum divisible by the n-th composite); computed by
 *            greedy simulation
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint8_t *A076065_bits = NULL;
static size_t A076065_bytes = 0;
static idx_t *A076065_fin = NULL;
static size_t A076065_cnt = 0, A076065_cap = 0;
static idx_t A076065_small = 1;
static idx_t A076065_comp = 3;

static bool A076065_used(idx_t x)
{
    size_t b = (size_t)(x >> 3);
    if (b >= A076065_bytes) return false;
    return (A076065_bits[b] >> (x & 7)) & 1;
}

static void A076065_mark(idx_t x)
{
    size_t b = (size_t)(x >> 3);
    if (b >= A076065_bytes) {
        size_t nl = A076065_bytes ? A076065_bytes : 256;
        while (nl <= b) nl <<= 1;
        uint8_t *t = (uint8_t *)realloc(A076065_bits, nl);
        if (!t) exit(1);
        memset(t + A076065_bytes, 0, nl - A076065_bytes);
        A076065_bits = t;
        A076065_bytes = nl;
    }
    A076065_bits[b] |= (uint8_t)(1u << (x & 7));
}

static idx_t A076065_next_composite(void)
{
    for (;;) {
        A076065_comp++;
        idx_t c = A076065_comp;
        for (idx_t d = 2; d * d <= c; d++)
            if (c % d == 0) return c;
    }
}

seq_t a(idx_t n)
{
    while (A076065_cnt < (size_t)n) {
        idx_t r = (idx_t)A076065_cnt + 1;
        idx_t comp = A076065_next_composite();
        idx_t sum = 0;
        for (idx_t i = 0; i + 1 < r; i++) {
            while (A076065_used(A076065_small)) A076065_small++;
            A076065_mark(A076065_small);
            sum = (sum + A076065_small) % comp;
            A076065_small++;
        }
        idx_t target = (comp - sum) % comp;
        idx_t cand = A076065_small;
        idx_t rem = cand % comp;
        idx_t diff = (target >= rem) ? (target - rem) : (target + comp - rem);
        cand += diff;
        while (A076065_used(cand)) cand += comp;
        A076065_mark(cand);
        if (A076065_cnt == A076065_cap) {
            size_t nc = A076065_cap ? A076065_cap << 1 : 256;
            idx_t *t = (idx_t *)realloc(A076065_fin, nc * sizeof(idx_t));
            if (!t) exit(1);
            A076065_fin = t;
            A076065_cap = nc;
        }
        A076065_fin[A076065_cnt++] = cand;
    }
    return (seq_t)A076065_fin[n - 1];
}
