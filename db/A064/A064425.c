/*
 * A064425  Gaps between where primes occur in A064413.
 * https://oeis.org/A064425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(n+1) - P(n), where P(k) is the index in the EKG
 *            sequence A064413 at which the k-th prime appears (primes occur
 *            in increasing order); computed by simulating A064413 until n+1
 *            primes have appeared
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 95 terms through n = 9288 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint32_t *A064425_spf = NULL;
static unsigned char *A064425_used = NULL;
static uint32_t *A064425_nxt = NULL;
static size_t A064425_cap = 0;

static uint64_t *A064425_pp = NULL;   /* 1-based positions of primes */
static size_t A064425_pcnt = 0, A064425_pcap = 0;

static uint32_t A064425_last = 0;
static uint64_t A064425_len = 0;

static void A064425_grow(size_t nc)
{
    size_t oc = A064425_cap;
    A064425_spf = (uint32_t *)realloc(A064425_spf, (nc + 1) * sizeof(uint32_t));
    A064425_used = (unsigned char *)realloc(A064425_used, nc + 1);
    A064425_nxt = (uint32_t *)realloc(A064425_nxt, (nc + 1) * sizeof(uint32_t));
    if (!A064425_spf || !A064425_used || !A064425_nxt) exit(1);
    if (oc == 0) {
        memset(A064425_used, 0, nc + 1);
        memset(A064425_nxt, 0, (nc + 1) * sizeof(uint32_t));
    } else {
        memset(A064425_used + oc + 1, 0, nc - oc);
        memset(A064425_nxt + oc + 1, 0, (nc - oc) * sizeof(uint32_t));
    }
    memset(A064425_spf, 0, (nc + 1) * sizeof(uint32_t));
    for (size_t i = 2; i <= nc; i++) {
        if (A064425_spf[i] == 0) {
            for (size_t j = i; j <= nc; j += i)
                if (A064425_spf[j] == 0) A064425_spf[j] = (uint32_t)i;
        }
    }
    A064425_cap = nc;
}

static void A064425_addprime(uint64_t pos)
{
    if (A064425_pcnt + 2 > A064425_pcap) {
        A064425_pcap = A064425_pcap ? A064425_pcap * 2 : 1024;
        A064425_pp = (uint64_t *)realloc(A064425_pp, A064425_pcap * sizeof(uint64_t));
        if (!A064425_pp) exit(1);
    }
    A064425_pp[++A064425_pcnt] = pos;
}

static void A064425_init(void)
{
    if (A064425_len) return;
    A064425_grow(1024);
    A064425_used[1] = 1;
    A064425_used[2] = 1;
    A064425_last = 2;
    A064425_len = 2;
    A064425_addprime(2);
}

static void A064425_step(void)
{
    uint32_t fac[16];
    int fc = 0;
    uint32_t x = A064425_last;
    while (x > 1) {
        uint32_t p = A064425_spf[x];
        fac[fc++] = p;
        while (x % p == 0) x /= p;
    }
    uint32_t best = UINT32_MAX;
    for (int i = 0; i < fc; i++) {
        uint32_t p = fac[i];
        uint32_t c = A064425_nxt[p] ? A064425_nxt[p] : p;
        for (;;) {
            if (c > A064425_cap) A064425_grow(A064425_cap * 2);
            if (!A064425_used[c]) break;
            c += p;
        }
        A064425_nxt[p] = c;
        if (c < best) best = c;
    }
    A064425_used[best] = 1;
    A064425_last = best;
    A064425_len++;
    if (A064425_spf[best] == best) A064425_addprime(A064425_len);
}

seq_t a(idx_t n)
{
    A064425_init();
    while (A064425_pcnt < (size_t)n + 1) A064425_step();
    return (seq_t)(A064425_pp[n + 1] - A064425_pp[n]);
}
