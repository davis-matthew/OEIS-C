/*
 * A073832  k between A001359(n) and A001359(n+1) such that A073830(k) is
 *          maximal.
 * https://oeis.org/A073832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = the k with A001359(n) < k < A001359(n+1) that
 *            maximizes A073830(k) = (4*((k-1)! + 1) + k) mod (k*(k+2)); the
 *            first such k on ties. Each A073830(k) is below k*(k+2), so it is
 *            computed exactly in native 64-bit arithmetic
 * Range:     u8 n <= 16, u16 n <= 860, u32 n <= 2600, u64 n <= 2600,
 *            u128 n <= 2600, big any n
 * Validated: standard, pass, 66 terms through n = 2219 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A001359_tw = NULL;
static size_t A001359_cnt = 0;
static uint64_t A001359_lim = 0;

static void A001359_extend(uint64_t newlim) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(newlim + 3));
    uint64_t i, j, top = newlim + 2;
    size_t cap = 1024, c = 0;
    uint64_t *list;
    memset(comp, 0, (size_t)(newlim + 3));
    comp[0] = comp[1] = 1;
    for (i = 2; i <= top / i; i++)
        if (!comp[i])
            for (j = i * i; j <= top; j += i) comp[j] = 1;
    list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 2; i <= newlim; i++) {
        if (!comp[i] && !comp[i + 2]) {
            if (c == cap) {
                cap *= 2;
                list = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
            }
            list[c++] = i;
        }
    }
    free(comp);
    free(A001359_tw);
    A001359_tw = list;
    A001359_cnt = c;
    A001359_lim = newlim;
}

static uint64_t A001359_get(uint64_t n) {
    while ((uint64_t)A001359_cnt < n) {
        uint64_t nl = A001359_lim ? A001359_lim * 2 : 4096;
        A001359_extend(nl);
    }
    return A001359_tw[n - 1];
}

/* A073830(k) in native arithmetic; the result is < k*(k+2). */
static uint64_t A073830_u64(uint64_t k)
{
    uint64_t mod, fact = 1, i;
    if (k <= 1)
        return 0;
    mod = k * (k + 2);
    if (k < 2000000ULL) {
        /* fact < mod < 2^42 and i < 2^21, so the product fits in 64 bits */
        for (i = 2; i < k && fact != 0; ++i)
            fact = (fact * i) % mod;
    } else {
        for (i = 2; i < k && fact != 0; ++i)
            fact = (uint64_t)(((unsigned __int128)fact * i) % mod);
    }
    return (uint64_t)(((unsigned __int128)4 * (fact + 1) + k) % mod);
}

seq_t a(idx_t n)
{
    uint64_t lo = A001359_get((uint64_t)n);
    uint64_t hi = A001359_get((uint64_t)n + 1);
    uint64_t bestk = lo + 1;
    uint64_t best = 0;
    bool have = false;
    uint64_t k;
    for (k = lo + 1; k < hi; ++k) {
        uint64_t v = A073830_u64(k);
        if (!have || v > best) {
            best = v;
            bestk = k;
            have = true;
        }
    }
    return (seq_t)bestk;
}
