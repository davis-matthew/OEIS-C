/*
 * A063463  Squarefree numbers arising in A024110.
 * https://oeis.org/A063463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   search: a(n) = n-th value of A024110(k) = 9^k - k^9, k >= 1,
 *            that is squarefree; squarefreeness is tested as 9^k != k^9 (mod
 *            p^2) for all primes p up to 300000, using native modular
 *            arithmetic instead of trial division of the big value
 * Range:     i8 none, i16 n <= 1, i32 n <= 4, i64 n <= 9, i128 n <= 20,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 41 (2026-11-07)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"

#define SQF_BOUND 300000u

static seq_t A024110_a(idx_t n)
{
    seq_t pow9n = ipow((seq_t)9, (seq_t)n);
    seq_t pown9 = ipow((seq_t)n, (seq_t)9);
    return pow9n - pown9;
}

static uint32_t *A063463_primes = NULL;
static size_t A063463_nprimes = 0;

static void A063463_init_primes(void)
{
    uint32_t i, j;
    unsigned char *comp;
    size_t cnt = 0;
    if (A063463_primes) return;
    comp = (unsigned char *)calloc(SQF_BOUND + 1, 1);
    A063463_primes = (uint32_t *)malloc(sizeof(uint32_t) * (SQF_BOUND / 2 + 16));
    for (i = 2; i <= SQF_BOUND; i++) {
        if (comp[i]) continue;
        A063463_primes[cnt++] = i;
        if ((uint64_t)i * i <= SQF_BOUND)
            for (j = i * i; j <= SQF_BOUND; j += i) comp[j] = 1;
    }
    free(comp);
    A063463_nprimes = cnt;
}

static uint64_t A063463_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A063463_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A063463_mulmod(r, b, m);
        b = A063463_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

/* true if 9^k - k^9 has no square factor p^2 with p <= SQF_BOUND (and is nonzero) */
static bool A063463_is_sqfree_k(uint64_t k)
{
    size_t i;
    if (k == 9) return false; /* value 0 */
    for (i = 0; i < A063463_nprimes; i++) {
        uint64_t p = A063463_primes[i];
        uint64_t m = p * p;
        if (A063463_powmod(9, k, m) == A063463_powmod(k, 9, m)) return false;
    }
    return true;
}

static uint64_t *A063463_ks = NULL;
static size_t A063463_cnt = 0, A063463_cap = 0;
static uint64_t A063463_next_k = 1;

seq_t a(idx_t n)
{
    A063463_init_primes();
    while (A063463_cnt < n) {
        uint64_t k = A063463_next_k++;
        if (A063463_is_sqfree_k(k)) {
            if (A063463_cnt == A063463_cap) {
                A063463_cap = A063463_cap ? 2 * A063463_cap : 64;
                A063463_ks = (uint64_t *)realloc(A063463_ks, sizeof(uint64_t) * A063463_cap);
            }
            A063463_ks[A063463_cnt++] = k;
        }
    }
    return A024110_a((idx_t)A063463_ks[n - 1]);
}
