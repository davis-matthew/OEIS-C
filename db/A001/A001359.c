/*
 * A001359  Lesser of twin primes.
 * https://oeis.org/A001359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   sieve of Eratosthenes; a(n) = n-th prime p such that p+2 is also
 *            prime
 * Range:     u8 n <= 17, u16 n <= 860, u32 n <= 9291, u64 n <= 9291,
 *            u128 n <= 9291, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *a001359_tw = NULL;
static size_t a001359_cnt = 0;
static uint64_t a001359_lim = 0;

static void a001359_extend(uint64_t newlim) {
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
    free(a001359_tw);
    a001359_tw = list;
    a001359_cnt = c;
    a001359_lim = newlim;
}

seq_t a(idx_t n) {
    while ((uint64_t)a001359_cnt < (uint64_t)n) {
        uint64_t nl = a001359_lim ? a001359_lim * 2 : 4096;
        a001359_extend(nl);
    }
    return (seq_t)a001359_tw[n - 1];
}
