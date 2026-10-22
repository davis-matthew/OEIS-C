/*
 * A001583  Artiads: the primes p == 1 (mod 5) for which Fibonacci((p-1)/5) is
 *          divisible by p.
 * https://oeis.org/A001583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search -- sieve the primes p == 1 (mod 5) and keep those with
 *            Fibonacci((p-1)/5) == 0 (mod p)
 * Range:     u8 n <= 1, u16 n <= 328, u32 n <= 9919, u64 n <= 9919,
 *            u128 n <= 9919, big any n
 * Helpers:   a001583_fib, a001583_mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a001583_fib.h"
#include "../utils/a001583_mulmod.h"

static uint64_t *a001583_terms = NULL;
static idx_t a001583_cnt = 0;
static uint64_t a001583_limit = 0;

static void a001583_extend(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j, cap;
    idx_t cnt = 0;
    uint64_t *t;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
    cap = 16;
    t = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (i = 11; i <= limit; i += 10) { /* odd and == 1 mod 5 */
        if (comp[i]) continue;
        if (a001583_fib((i - 1) / 5, i) == 0) {
            if (cnt == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, (size_t)cap * sizeof(uint64_t));
            }
            t[cnt++] = i;
        }
    }
    free(comp);
    free(a001583_terms);
    a001583_terms = t;
    a001583_cnt = cnt;
    a001583_limit = limit;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (a001583_cnt < n) {
        uint64_t lim = a001583_limit ? a001583_limit * 2 : 100000;
        a001583_extend(lim);
    }
    return (seq_t)a001583_terms[n - 1];
}
