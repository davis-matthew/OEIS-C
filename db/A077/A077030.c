/*
 * A077030  Numbers k such that A068340(k)=+/-2.
 * https://oeis.org/A077030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = n-th k such that A068340(k) = Sum_{j<=k} mu(j)*j equals
 *            +2 or -2; computed by an incremental segmented Moebius sieve
 *            search
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A077030_BLK (1u << 18)
#define A077030_PLIM 2000000u

static uint32_t *A077030_pr = NULL;
static size_t A077030_np = 0;
static uint64_t A077030_next = 1;      /* next k to process */
static int64_t A077030_sum = 0;
static uint64_t *A077030_found = NULL;
static size_t A077030_nf = 0, A077030_cap = 0;

static void A077030_init(void)
{
    if (A077030_pr) return;
    uint8_t *c = (uint8_t *)malloc(A077030_PLIM + 1);
    memset(c, 0, A077030_PLIM + 1);
    A077030_pr = (uint32_t *)malloc(sizeof(uint32_t) * 150000);
    for (uint32_t i = 2; i <= A077030_PLIM; i++) {
        if (!c[i]) {
            A077030_pr[A077030_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A077030_PLIM; j += i) c[j] = 1;
        }
    }
    free(c);
}

static void A077030_block(void)
{
    static uint64_t *val = NULL;
    static int8_t *mu = NULL;
    if (!val) {
        val = (uint64_t *)malloc(sizeof(uint64_t) * A077030_BLK);
        mu = (int8_t *)malloc(A077030_BLK);
    }
    uint64_t lo = A077030_next, hi = lo + A077030_BLK;
    for (uint32_t i = 0; i < A077030_BLK; i++) { val[i] = lo + i; mu[i] = 1; }
    for (size_t pi = 0; pi < A077030_np; pi++) {
        uint64_t p = A077030_pr[pi];
        if (p * p >= hi) break;
        uint64_t first = ((lo + p - 1) / p) * p;
        for (uint64_t m = first; m < hi; m += p) {
            uint32_t i = (uint32_t)(m - lo);
            val[i] /= p;
            mu[i] = (int8_t)-mu[i];
        }
        uint64_t p2 = p * p;
        first = ((lo + p2 - 1) / p2) * p2;
        for (uint64_t m = first; m < hi; m += p2) mu[(uint32_t)(m - lo)] = 0;
    }
    for (uint32_t i = 0; i < A077030_BLK; i++) {
        int m = mu[i];
        if (m != 0 && val[i] > 1) m = -m;
        uint64_t k = lo + i;
        A077030_sum += (int64_t)m * (int64_t)k;
        if (A077030_sum == 2 || A077030_sum == -2) {
            if (A077030_nf == A077030_cap) {
                A077030_cap = A077030_cap ? A077030_cap * 2 : 64;
                A077030_found = (uint64_t *)realloc(A077030_found, sizeof(uint64_t) * A077030_cap);
            }
            A077030_found[A077030_nf++] = k;
        }
    }
    A077030_next = hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A077030_init();
    while (A077030_nf < (size_t)n) A077030_block();
    return (seq_t)A077030_found[n - 1];
}
