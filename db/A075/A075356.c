/*
 * A075356  Sum of terms in n-th group in A075352.
 * https://oeis.org/A075356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A075355(n)*(A075354(n)+A075353(n))/2, i.e. the sum of the
 *            n-th group of consecutive integers in A075352; groups are found
 *            greedily with exact big-integer comparison of group product vs
 *            n!
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

typedef struct { uint32_t *d; size_t len, cap; } A075356_bn;

static void A075356_set1(A075356_bn *b)
{
    if (b->cap == 0) {
        b->cap = 16;
        b->d = (uint32_t *)malloc(b->cap * sizeof(uint32_t));
    }
    b->d[0] = 1;
    b->len = 1;
}

static void A075356_mul(A075356_bn *b, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t t = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        if (b->len == b->cap) {
            b->cap *= 2;
            b->d = (uint32_t *)realloc(b->d, b->cap * sizeof(uint32_t));
        }
        b->d[b->len++] = (uint32_t)carry;
    }
}

static int A075356_ge(const A075356_bn *a, const A075356_bn *b)
{
    if (a->len != b->len) return a->len > b->len;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return a->d[i] > b->d[i];
    }
    return 1;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_start = 1;
    static A075356_bn fact = { NULL, 0, 0 };
    static A075356_bn prod = { NULL, 0, 0 };

    if (n <= cache_len) return cache[n - 1];

    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }

    if (fact.cap == 0) A075356_set1(&fact);

    for (idx_t k = cache_len + 1; k <= n; ++k) {
        A075356_mul(&fact, (uint32_t)k);
        A075356_set1(&prod);
        uint64_t start = next_start;
        for (uint64_t m = start; ; ++m) {
            A075356_mul(&prod, (uint32_t)m);
            if (A075356_ge(&prod, &fact)) {
                seq_t cnt = (seq_t)(m - start + 1);
                seq_t s = (seq_t)start + (seq_t)m;
                cache[k - 1] = s * cnt / 2;
                next_start = m + 1;
                break;
            }
        }
        cache_len = (size_t)k;
    }
    return cache[n - 1];
}
