/*
 * A002367  Let p = A007645(n) be the n-th generalized cuban prime and write
 *          p^2 = x^2 + 3*y^2 with y > 0; a(n) = x.
 * https://oeis.org/A002367
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   p = A007645(n) (3 and the primes == 1 mod 3, found with a
 *            sieve); write p = u^2 + 3*v^2; then a(n) = |u^2 - 3*v^2| (since
 *            p^2 = (u^2-3v^2)^2 + 3*(2uv)^2)
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/a000037_isqrt.h"

static uint64_t *cp_list = NULL;
static size_t cp_cnt = 0;
static uint64_t cp_limit = 0;

static void cp_build(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)limit + 1);
    memset(comp, 0, (size_t)limit + 1);
    size_t cap = 1024, cnt = 0;
    uint64_t *lst = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t i = 2; i <= limit; i++) {
        if (comp[i]) continue;
        if (i * i <= limit)
            for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
        if (i == 3 || i % 3 == 1) {
            if (cnt == cap) {
                cap *= 2;
                lst = (uint64_t *)realloc(lst, cap * sizeof(uint64_t));
            }
            lst[cnt++] = i;
        }
    }
    free(comp);
    free(cp_list);
    cp_list = lst;
    cp_cnt = cnt;
    cp_limit = limit;
}

seq_t a(idx_t n) {
    uint64_t limit = cp_limit ? cp_limit : 65536;
    while (cp_cnt < (size_t)n) {
        if (cp_limit) limit *= 2;
        cp_build(limit);
    }
    uint64_t p = cp_list[n - 1];
    for (uint64_t v = 1; 3 * v * v < p; v++) {
        uint64_t r = p - 3 * v * v;
        uint64_t u = a000037_isqrt(r);
        if (u * u == r) {
            uint64_t s = u * u, t = 3 * v * v;
            return (seq_t)(s > t ? s - t : t - s);
        }
    }
    return (seq_t)p;
}
