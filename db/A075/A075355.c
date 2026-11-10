/*
 * A075355  Number of terms in n-th group in A075352.
 * https://oeis.org/A075355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A075354(n)-A075353(n)+1, the size of the n-th group when
 *            the naturals are grouped consecutively so each group's product
 *            is the smallest >= n! (exact bignum search)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t *d;
    size_t len;
    size_t cap;
} A075355_big;

static void A075355_reserve(A075355_big *b, size_t need)
{
    if (b->cap < need) {
        size_t nc = b->cap ? b->cap * 2 : 8;
        while (nc < need) nc *= 2;
        b->d = (uint32_t *)realloc(b->d, nc * sizeof(uint32_t));
        b->cap = nc;
    }
}

static void A075355_set1(A075355_big *b)
{
    A075355_reserve(b, 1);
    b->d[0] = 1;
    b->len = 1;
}

static void A075355_mul(A075355_big *b, uint64_t m)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < b->len; i++) {
        /* m < 2^32 assumed */
        uint64_t t = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    while (carry) {
        A075355_reserve(b, b->len + 1);
        b->d[b->len++] = (uint32_t)carry;
        carry >>= 32;
    }
}

static int A075355_ge(const A075355_big *x, const A075355_big *y)
{
    size_t i;
    if (x->len != y->len) return x->len > y->len;
    for (i = x->len; i-- > 0;) {
        if (x->d[i] != y->d[i]) return x->d[i] > y->d[i];
    }
    return 1;
}

seq_t a(idx_t n)
{
    static uint32_t *cnt = NULL;
    static size_t cnt_len = 0, cnt_cap = 0;
    static A075355_big F = {NULL, 0, 0};
    static A075355_big P = {NULL, 0, 0};
    static uint64_t next_start = 1;

    if (F.len == 0) A075355_set1(&F);

    while ((uint64_t)cnt_len < (uint64_t)n) {
        uint64_t k = (uint64_t)cnt_len + 1;
        uint32_t c = 0;
        uint64_t m;
        A075355_mul(&F, k);
        A075355_set1(&P);
        for (m = next_start;; m++) {
            A075355_mul(&P, m);
            c++;
            if (A075355_ge(&P, &F)) break;
        }
        next_start = m + 1;
        if (cnt_len >= cnt_cap) {
            cnt_cap = cnt_cap ? cnt_cap * 2 : 64;
            cnt = (uint32_t *)realloc(cnt, cnt_cap * sizeof(uint32_t));
        }
        cnt[cnt_len++] = c;
    }
    return (seq_t)cnt[n - 1];
}
