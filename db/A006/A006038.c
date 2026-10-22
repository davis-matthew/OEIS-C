/*
 * A006038  Odd primitive abundant numbers.
 * https://oeis.org/A006038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve over odd n, a(n) = n-th odd n with sigma(n) > 2n and
 *            sigma(n/p) <= 2n/p for every prime p | n (enough to check the p
 *            maximizing sigma(p^e))
 * Range:     u8 none, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SEG 65536

static uint64_t *res = NULL;
static size_t rcount = 0, rcap = 0;
static uint64_t next_lo = 1;

static uint64_t *primes = NULL;
static size_t nprimes = 0;
static uint64_t plimit = 0;

static uint64_t *rem_a, *sig_a, *best_a, *bprev_a, *bp_a;

static void ensure_primes(uint64_t lim) {
    if (plimit >= lim) return;
    uint64_t nl = lim;
    if (nl < 2 * plimit) nl = 2 * plimit;
    if (nl < 1024) nl = 1024;
    char *comp = (char *)calloc((size_t)nl + 1, 1);
    free(primes);
    primes = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)nl / 2 + 2));
    nprimes = 0;
    for (uint64_t i = 3; i <= nl; i += 2) {
        if (!comp[i]) {
            primes[nprimes++] = i;
            for (uint64_t j = i * i; j <= nl; j += 2 * i) comp[j] = 1;
        }
    }
    free(comp);
    plimit = nl;
}

static void push_res(uint64_t v) {
    if (rcount == rcap) {
        rcap = rcap ? rcap * 2 : 1024;
        res = (uint64_t *)realloc(res, sizeof(uint64_t) * rcap);
    }
    res[rcount++] = v;
}

static void do_segment(void) {
    if (!rem_a) {
        rem_a = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
        sig_a = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
        best_a = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
        bprev_a = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
        bp_a = (uint64_t *)malloc(sizeof(uint64_t) * SEG);
    }
    uint64_t lo = next_lo;
    uint64_t hi = lo + 2 * (uint64_t)(SEG - 1);
    uint64_t sq = 1;
    while ((sq + 1) * (sq + 1) <= hi) sq++;
    ensure_primes(sq + 1);
    for (size_t i = 0; i < SEG; i++) {
        rem_a[i] = lo + 2 * (uint64_t)i;
        sig_a[i] = 1;
        best_a[i] = 0;
        bprev_a[i] = 1;
        bp_a[i] = 1;
    }
    for (size_t k = 0; k < nprimes; k++) {
        uint64_t p = primes[k];
        if (p > sq) break;
        uint64_t m = ((lo + p - 1) / p) * p;
        if ((m & 1) == 0) m += p;
        for (uint64_t idx = (m - lo) / 2; idx < SEG; idx += p) {
            uint64_t r = rem_a[idx];
            uint64_t pe = 1, sg = 1, prev = 1;
            while (r % p == 0) {
                r /= p;
                prev = sg;
                pe *= p;
                sg += pe;
            }
            rem_a[idx] = r;
            sig_a[idx] *= sg;
            if (sg > best_a[idx]) {
                best_a[idx] = sg;
                bprev_a[idx] = prev;
                bp_a[idx] = p;
            }
        }
    }
    for (size_t i = 0; i < SEG; i++) {
        uint64_t n = lo + 2 * (uint64_t)i;
        if (n < 3) continue;
        uint64_t r = rem_a[i];
        if (r > 1) {
            uint64_t sg = r + 1;
            sig_a[i] *= sg;
            if (sg > best_a[i]) {
                best_a[i] = sg;
                bprev_a[i] = 1;
                bp_a[i] = r;
            }
        }
        uint64_t s = sig_a[i];
        if (s <= 2 * n) continue;
        uint64_t sd = s / best_a[i] * bprev_a[i];
        if (sd <= 2 * (n / bp_a[i])) push_res(n);
    }
    next_lo = hi + 2;
}

seq_t a(idx_t n) {
    while (rcount < n) do_segment();
    return (seq_t)res[n - 1];
}
