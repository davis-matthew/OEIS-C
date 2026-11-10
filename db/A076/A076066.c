/*
 * A076066  Sums of members of groups in A076063.
 * https://oeis.org/A076066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the n elements of the n-th group of A076063 (the
 *            n-th group's sum is a multiple of the n-th composite); computed
 *            by greedy simulation of the grouping
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A076066_sums = NULL;
static size_t A076066_len = 0;
static size_t A076066_cap = 0;
static uint64_t A076066_smallest = 1;
static uint64_t A076066_comp = 3;
static uint8_t *A076066_bits = NULL;
static size_t A076066_bytes = 0;

static bool A076066_used(uint64_t x)
{
    size_t b = (size_t)(x >> 3);
    if (b >= A076066_bytes) return false;
    return (A076066_bits[b] >> (x & 7)) & 1;
}

static void A076066_mark(uint64_t x)
{
    size_t b = (size_t)(x >> 3);
    if (b >= A076066_bytes) {
        size_t nl = A076066_bytes ? A076066_bytes : 128;
        while (nl <= b) nl <<= 1;
        uint8_t *t = (uint8_t *)realloc(A076066_bits, nl);
        if (!t) exit(1);
        memset(t + A076066_bytes, 0, nl - A076066_bytes);
        A076066_bits = t;
        A076066_bytes = nl;
    }
    A076066_bits[b] |= (uint8_t)(1u << (x & 7));
}

static void A076066_next_composite(void)
{
    for (;;) {
        A076066_comp++;
        uint64_t c = A076066_comp;
        for (uint64_t d = 2; d * d <= c; d++)
            if (c % d == 0) return;
    }
}

seq_t a(idx_t n)
{
    while (A076066_len < (size_t)n) {
        uint64_t r = (uint64_t)A076066_len + 1;
        A076066_next_composite();
        uint64_t comp = A076066_comp;
        uint64_t sm = 0;
        seq_t total = 0;
        for (uint64_t i = 0; i + 1 < r; i++) {
            while (A076066_used(A076066_smallest)) A076066_smallest++;
            uint64_t v = A076066_smallest;
            A076066_mark(v);
            A076066_smallest++;
            total += (seq_t)v;
            sm = (sm + v) % comp;
        }
        while (A076066_used(A076066_smallest)) A076066_smallest++;
        uint64_t target = (comp - sm) % comp;
        uint64_t cand = A076066_smallest;
        uint64_t rem = cand % comp;
        uint64_t diff = (target >= rem) ? (target - rem) : (target + comp - rem);
        cand += diff;
        while (A076066_used(cand)) cand += comp;
        A076066_mark(cand);
        total += (seq_t)cand;
        if (A076066_len == A076066_cap) {
            size_t nc = A076066_cap ? A076066_cap << 1 : 256;
            A076066_sums = mem_resize(seq_t, A076066_sums, nc);
            if (!A076066_sums) exit(1);
            A076066_cap = nc;
        }
        A076066_sums[A076066_len++] = total;
    }
    return A076066_sums[n - 1];
}
