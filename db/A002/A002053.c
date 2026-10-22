/*
 * A002053  a(n) = least value of m for which Liouville's function A002819(m)
 *          = -n.
 * https://oeis.org/A002053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   a(n) = least m with A002819(m) = sum_{k<=m} (-1)^Omega(k) = -n.
 *            Found by a segmented sieve of Liouville's function that records
 *            each new minimum of the running sum
 * Range:     u8 n <= 16, u16 n <= 309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SEG 262144
#define PLIM 1048576

static uint32_t *pr = NULL;
static size_t npr = 0;
static uint64_t *res = NULL;
static size_t rcnt = 0, rcap = 0;
static uint64_t next_m = 1;
static long long Lsum = 0, minL = 1;
static uint64_t *prodv = NULL;
static unsigned char *par = NULL;

static void init_primes(void) {
    unsigned char *c = (unsigned char *)calloc(PLIM + 1, 1);
    size_t i, j;
    pr = (uint32_t *)malloc(sizeof(uint32_t) * 100000);
    for (i = 2; i <= PLIM; i++) {
        if (!c[i]) {
            pr[npr++] = (uint32_t)i;
            for (j = i * i; j <= PLIM; j += i) c[j] = 1;
        }
    }
    free(c);
    prodv = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
    par = (unsigned char *)malloc(SEG);
}

static void push_rec(uint64_t m) {
    if (rcnt == rcap) {
        rcap = rcap ? rcap * 2 : 1024;
        res = (uint64_t *)realloc(res, sizeof(uint64_t) * rcap);
    }
    res[rcnt++] = m;
}

static void do_segment(void) {
    uint64_t lo = next_m, hi = lo + SEG;
    size_t i, k;
    for (i = 0; i < SEG; i++) { prodv[i] = 1; par[i] = 0; }
    for (k = 0; k < npr; k++) {
        uint64_t p = pr[k];
        uint64_t pk;
        if (p * p >= hi) break;
        for (pk = p; pk < hi; pk *= p) {
            uint64_t s = ((lo + pk - 1) / pk) * pk;
            uint64_t j;
            for (j = s; j < hi; j += pk) {
                par[j - lo] ^= 1;
                prodv[j - lo] *= p;
            }
            if (pk > hi / p) break;
        }
    }
    for (i = 0; i < SEG; i++) {
        uint64_t m = lo + i;
        int pa = par[i];
        if (prodv[i] < m) pa ^= 1;
        Lsum += pa ? -1 : 1;
        if (Lsum < minL) {
            minL = Lsum;
            push_rec(m);
        }
    }
    next_m = hi;
}

seq_t a(idx_t n) {
    if (!pr) init_primes();
    while ((idx_t)rcnt <= n) do_segment();
    return (seq_t)res[n];
}
