/*
 * A003147  Primes p with a Fibonacci primitive root g, i.e., such that g^2 =
 *          g + 1 (mod p).
 * https://oeis.org/A003147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search: a(n) = n-th prime p with a root g of g^2 = g + 1 (mod p)
 *            that is a primitive root mod p. Except for p = 5, this needs p
 *            == +-1 (mod 10); then g = (1 +- sqrt(5))/2 mod p, found by
 *            Tonelli-Shanks
 * Range:     u8 n <= 17, u16 n <= 1749, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a003147_isprim, a003147_ok, a003147_pm, a003147_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a003147_isprim.h"
#include "../utils/a003147_ok.h"
#include "../utils/a003147_pm.h"
#include "../utils/a003147_sqrt.h"

static uint64_t *a003147_terms = NULL;
static idx_t a003147_cnt = 0;
static uint64_t a003147_limit = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (a003147_cnt < n) {
        uint64_t newlim = a003147_limit ? a003147_limit * 2 : 65536;
        unsigned char *comp = (unsigned char *)calloc((size_t)newlim + 1, 1);
        uint64_t i, j;
        idx_t cap = 1024, cnt = 0;
        uint64_t *terms = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
        for (i = 2; i <= newlim; i++) {
            if (comp[i]) continue;
            for (j = i * i; j <= newlim; j += i) comp[j] = 1;
            if (a003147_ok(i)) {
                if (cnt == cap) {
                    cap *= 2;
                    terms = (uint64_t *)realloc(terms, (size_t)cap * sizeof(uint64_t));
                }
                terms[cnt++] = i;
            }
        }
        free(comp);
        free(a003147_terms);
        a003147_terms = terms;
        a003147_cnt = cnt;
        a003147_limit = newlim;
    }
    return (seq_t)a003147_terms[n - 1];
}
