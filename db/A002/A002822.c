/*
 * A002822  Numbers m such that 6m-1, 6m+1 are twin primes.
 * https://oeis.org/A002822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 3750, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve search: a(n) = n-th m such that 6m-1 and 6m+1 are both prime (a(n) = A014574(n+1)/6) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a002822_terms = NULL;
static idx_t a002822_count = 0;
static uint64_t a002822_limit = 0;

static void a002822_build(uint64_t mmax) {
    uint64_t N = 6 * mmax + 2;
    unsigned char *comp = (unsigned char *)malloc((size_t)N + 1);
    uint64_t i, j, m;
    idx_t cnt = 0, cap = 1024;
    uint64_t *t = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    memset(comp, 0, (size_t)N + 1);
    comp[0] = 1;
    comp[1] = 1;
    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i)
                comp[j] = 1;
    for (m = 1; m <= mmax; m++) {
        if (!comp[6 * m - 1] && !comp[6 * m + 1]) {
            if (cnt == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, (size_t)cap * sizeof(uint64_t));
            }
            t[cnt++] = m;
        }
    }
    free(comp);
    free(a002822_terms);
    a002822_terms = t;
    a002822_count = cnt;
    a002822_limit = mmax;
}

seq_t a(idx_t n) {
    uint64_t lim = a002822_limit ? a002822_limit : 4096;
    if (a002822_count < n) {
        while (a002822_count < n) {
            if (a002822_limit) lim *= 2;
            a002822_build(lim);
        }
    }
    return (seq_t)a002822_terms[n - 1];
}
