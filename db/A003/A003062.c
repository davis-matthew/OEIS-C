/*
 * A003062  Beginnings of periodic unitary aliquot sequences.
 * https://oeis.org/A003062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed formula. Take each m = 1, 2, 3, ... in turn
 *            and iterate s*(k) = A034460(k) = usigma(k) - k from k = m. If
 *            the iteration reaches 0, m is not a term. If it repeats a value
 *            (enters a cycle), m is a term. Results are cached for every
 *            value seen, and big numbers are factored with...
 * Range:     u8 n <= 21, u16 n <= 7296, u32 n <= 9966, u64 n <= 9966,
 *            u128 n <= 9966, big any n
 * Helpers:   u_factor, u_gcd, u_isprime, u_mulmod, u_powmod, u_rho, u_sstar
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/u_factor.h"
#include "../utils/u_gcd.h"
#include "../utils/u_isprime.h"
#include "../utils/u_mulmod.h"
#include "../utils/u_powmod.h"
#include "../utils/u_rho.h"
#include "../utils/u_sstar.h"

#define HBITS 18
#define HSIZE (1u << HBITS)
#define PCAP 100000

static unsigned char *st = NULL;   /* 0 unknown, 1 periodic, 2 not */
static uint64_t stcap = 0;
static uint64_t *hkeys = NULL;
static uint32_t *hstamp = NULL;
static uint32_t curstamp = 0;
static uint64_t *path = NULL;
static uint64_t *terms = NULL;
static idx_t tcnt = 0, tcap = 0;
static uint64_t nextm = 1;

static void ensure_st(uint64_t need) {
    uint64_t nc;
    if (need < stcap) return;
    nc = need * 2 + 65536;
    st = (unsigned char *)realloc(st, (size_t)nc);
    memset(st + stcap, 0, (size_t)(nc - stcap));
    stcap = nc;
}

/* returns true if x was already present */
static bool h_insert(uint64_t x) {
    uint32_t h = (uint32_t)((x * 0x9E3779B97F4A7C15ULL) >> (64 - HBITS));
    while (hstamp[h] == curstamp) {
        if (hkeys[h] == x) return true;
        h = (h + 1) & (HSIZE - 1);
    }
    hstamp[h] = curstamp;
    hkeys[h] = x;
    return false;
}

static unsigned char classify(uint64_t m) {
    uint64_t x = m;
    long len = 0, i;
    unsigned char res;
    if (m < stcap && st[m]) return st[m];
    curstamp++;
    if (curstamp == 0) { memset(hstamp, 0, sizeof(uint32_t) * HSIZE); curstamp = 1; }
    for (;;) {
        uint64_t nx;
        if (x <= 1) { res = 2; break; }
        if (x < stcap && st[x]) { res = st[x]; break; }
        if (h_insert(x)) { res = 1; break; }
        path[len++] = x;
        if (len >= PCAP) { res = 2; break; }
        if (!u_sstar(x, &nx)) { res = 2; break; }
        x = nx;
    }
    for (i = 0; i < len; i++) if (path[i] < stcap) st[path[i]] = res;
    return res;
}

seq_t a(idx_t n) {
    if (hkeys == NULL) {
        hkeys = (uint64_t *)malloc(sizeof(uint64_t) * HSIZE);
        hstamp = (uint32_t *)calloc(HSIZE, sizeof(uint32_t));
        path = (uint64_t *)malloc(sizeof(uint64_t) * PCAP);
    }
    while (tcnt < n) {
        uint64_t m = nextm++;
        ensure_st(m);
        if (classify(m) == 1) {
            if (tcnt == tcap) {
                tcap = tcap ? tcap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, sizeof(uint64_t) * (size_t)tcap);
            }
            terms[tcnt++] = m;
        }
    }
    return (seq_t)terms[n - 1];
}
