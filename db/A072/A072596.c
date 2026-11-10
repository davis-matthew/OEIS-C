/*
 * A072596  Nonsquares with A072594(n) = 0.
 * https://oeis.org/A072596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th nonsquare m with A072594(m) = 0, where A072594(m) is
 *            the XOR of the primes p with odd exponent in m; found by a
 *            segmented sieve search in increasing m
 * Range:     u8 n <= 1, u16 n <= 93, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9355 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A072596_BLOCK (1u << 16)

static uint64_t *A072596_terms = NULL;
static uint64_t A072596_cnt = 0, A072596_cap = 0;
static uint64_t A072596_next_lo = 2;
static uint64_t A072596_s = 1;               /* floor(sqrt(current m)) tracker */

static uint32_t *A072596_primes = NULL;
static uint64_t A072596_pc = 0, A072596_plim = 0;

static uint64_t A072596_x[A072596_BLOCK];
static uint64_t A072596_pr[A072596_BLOCK];

static void A072596_ensure_primes(uint64_t lim)
{
    uint64_t newlim, i, j;
    unsigned char *comp;
    if (A072596_plim >= lim) return;
    newlim = lim;
    if (newlim < 2 * A072596_plim) newlim = 2 * A072596_plim;
    if (newlim < 1024) newlim = 1024;
    comp = (unsigned char *)calloc((size_t)(newlim + 1), 1);
    free(A072596_primes);
    A072596_primes = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(newlim / 2 + 2));
    A072596_pc = 0;
    for (i = 2; i <= newlim; i++) {
        if (comp[i]) continue;
        A072596_primes[A072596_pc++] = (uint32_t)i;
        for (j = i * i; j <= newlim; j += i) comp[j] = 1;
    }
    free(comp);
    A072596_plim = newlim;
}

static void A072596_push(uint64_t m)
{
    if (A072596_cnt == A072596_cap) {
        A072596_cap = A072596_cap ? A072596_cap * 2 : 1024;
        A072596_terms = (uint64_t *)realloc(A072596_terms,
                                            sizeof(uint64_t) * (size_t)A072596_cap);
    }
    A072596_terms[A072596_cnt++] = m;
}

static void A072596_process_block(void)
{
    uint64_t lo = A072596_next_lo;
    uint64_t hi = lo + A072596_BLOCK;       /* exclusive */
    uint64_t r, i, k;

    r = 1;
    while ((r + 1) * (r + 1) < hi) r++;
    A072596_ensure_primes(r + 1);

    for (i = 0; i < A072596_BLOCK; i++) {
        A072596_x[i] = 0;
        A072596_pr[i] = 1;
    }

    for (k = 0; k < A072596_pc; k++) {
        uint64_t p = A072596_primes[k];
        uint64_t q;
        if (p * p >= hi) break;
        for (q = p; ; q *= p) {
            uint64_t m = ((lo + q - 1) / q) * q;
            for (; m < hi; m += q) {
                A072596_x[m - lo] ^= p;
                A072596_pr[m - lo] *= p;
            }
            if (q > (hi - 1) / p) break;
        }
    }

    for (i = 0; i < A072596_BLOCK; i++) {
        uint64_t m = lo + i;
        uint64_t rem = m / A072596_pr[i];
        uint64_t xv = A072596_x[i];
        if (rem > 1) xv ^= rem;
        while ((A072596_s + 1) * (A072596_s + 1) <= m) A072596_s++;
        if (xv == 0 && A072596_s * A072596_s != m) A072596_push(m);
    }

    A072596_next_lo = hi;
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;
    if (k == 0) k = 1;
    while (A072596_cnt < k) A072596_process_block();
    return (seq_t)A072596_terms[k - 1];
}
