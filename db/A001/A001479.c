/*
 * A001479  Let p = A007645(n) be the n-th generalized cuban prime and write p
 *          = x^2 + 3*y^2; a(n) = x.
 * https://oeis.org/A001479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve the primes p with p = 3 or p == 1 (mod 3) (A007645); a(n)
 *            = x, where the n-th such p = x^2 + 3*y^2, found by searching
 *            over y
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a1479_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a1479_isqrt.h"

static uint64_t *a1479_P = NULL;
static idx_t a1479_cnt = 0;
static uint64_t a1479_lim = 0;

static void a1479_build(uint64_t limit)
{
    bool *comp = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    memset(comp, 0, (size_t)(limit + 1) * sizeof(bool));
    uint64_t i, j, c = 0;
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = true;
    for (i = 2; i <= limit; i++)
        if (!comp[i] && (i == 3 || i % 3 == 1)) c++;
    free(a1479_P);
    a1479_P = (uint64_t *)malloc((size_t)(c + 1) * sizeof(uint64_t));
    c = 0;
    for (i = 2; i <= limit; i++)
        if (!comp[i] && (i == 3 || i % 3 == 1)) a1479_P[c++] = i;
    free(comp);
    a1479_cnt = (idx_t)c;
    a1479_lim = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (a1479_cnt < n) {
        uint64_t est = 1000, k = (uint64_t)n, lg = 1;
        while ((1ULL << lg) < k) lg++;
        /* p_n of A007645 is about 2 n ln n; lg*0.7 ~ ln n */
        if (k * lg * 2 + 1000 > est) est = k * lg * 2 + 1000;
        if (est < 2 * a1479_lim) est = 2 * a1479_lim;
        a1479_build(est);
        while (a1479_cnt < n) a1479_build(a1479_lim * 2);
    }
    uint64_t p = a1479_P[n - 1];
    uint64_t y;
    for (y = 0; 3 * y * y <= p; y++) {
        uint64_t r = p - 3 * y * y;
        uint64_t x = a1479_isqrt(r);
        if (x * x == r) return (seq_t)x;
    }
    return 0;
}
