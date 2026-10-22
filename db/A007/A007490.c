/*
 * A007490  Primes of form x^3 + y^3 + z^3 where x,y,z > 0.
 * https://oeis.org/A007490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: mark every x^3+y^3+z^3 (0<x<=y<=z) up to a limit, keep
 *            the primes in increasing order, and double the limit when more
 *            terms are needed
 * Range:     u8 n <= 9, u16 n <= 765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t limit = 0;

static void build(uint64_t L) {
    size_t words = (size_t)(L / 64 + 1);
    uint64_t *mark = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t x, y, z, s;
    idx_t cap = 1024, cnt = 0;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (x = 1; 3 * x * x * x <= L; x++) {
        uint64_t x3 = x * x * x;
        for (y = x; x3 + 2 * y * y * y <= L; y++) {
            uint64_t xy = x3 + y * y * y;
            for (z = y; (s = xy + z * z * z) <= L; z++)
                mark[s >> 6] |= (uint64_t)1 << (s & 63);
        }
    }
    for (s = 2; s <= L; s++) {
        if ((mark[s >> 6] >> (s & 63)) & 1) {
            if (is_prime(s)) {
                if (cnt == cap) {
                    cap *= 2;
                    t = (uint64_t *)realloc(t, cap * sizeof(uint64_t));
                }
                t[cnt++] = s;
            }
        }
    }
    free(mark);
    free(terms);
    terms = t;
    nterms = cnt;
    limit = L;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (limit == 0) build((uint64_t)1 << 16);
    while (n > nterms) build(limit * 2);
    return (seq_t)terms[n - 1];
}
