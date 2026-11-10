/*
 * A239070  Smallest m such that A132995(m) = n-th squarefree number.
 * https://oeis.org/A239070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A132995(m) = A005117(n), where A132995(m) =
 *            gcd(Sum_{k<=m} prime(k), Product_{k<=m} prime(k)) = product of
 *            the distinct prime factors <= prime(m) of the sum; found by
 *            incremental search over m with a first-occurrence hash table
 * Range:     u8 n <= 15, u16 n <= 440, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 70 terms through n = 1929 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static bool A239070_sqfree(uint64_t k) {
    if (k % 4 == 0) return false;
    for (uint64_t i = 3; i * i <= k; i += 2)
        if (k % (i * i) == 0) return false;
    return true;
}

static uint64_t A239070_nth_sqfree(idx_t n) {
    static uint64_t *cache = 0;
    static idx_t sz = 0, cap = 0;
    if (n > sz) {
        if (n > cap) {
            idx_t nc = cap ? cap : 64;
            while (nc < n) nc *= 2;
            cache = (uint64_t *)realloc(cache, (size_t)nc * sizeof(uint64_t));
            cap = nc;
        }
        uint64_t k = sz ? cache[sz - 1] : 0;
        for (idx_t i = sz; i < n; i++) {
            do { k++; } while (!A239070_sqfree(k));
            cache[i] = k;
        }
        sz = n;
    }
    return cache[n - 1];
}

static uint64_t *A239070_pr = 0;
static size_t A239070_np = 0, A239070_pcap = 0;
static uint64_t *A239070_hk = 0, *A239070_hv = 0;
static size_t A239070_hcap = 0, A239070_hcnt = 0;
static uint64_t A239070_sum = 0, A239070_m = 0, A239070_cand = 3;

static void A239070_hput(uint64_t key, uint64_t val);

static void A239070_hgrow(void) {
    size_t oc = A239070_hcap;
    uint64_t *ok = A239070_hk, *ov = A239070_hv;
    A239070_hcap = oc ? oc * 2 : 1024;
    A239070_hk = (uint64_t *)calloc(A239070_hcap, sizeof(uint64_t));
    A239070_hv = (uint64_t *)calloc(A239070_hcap, sizeof(uint64_t));
    A239070_hcnt = 0;
    for (size_t i = 0; i < oc; i++)
        if (ok[i]) A239070_hput(ok[i], ov[i]);
    free(ok); free(ov);
}

static void A239070_hput(uint64_t key, uint64_t val) {
    if ((A239070_hcnt + 1) * 2 > A239070_hcap) A239070_hgrow();
    size_t i = (size_t)((key * 11400714819323198485ULL) >> 20) & (A239070_hcap - 1);
    while (A239070_hk[i]) {
        if (A239070_hk[i] == key) return;
        i = (i + 1) & (A239070_hcap - 1);
    }
    A239070_hk[i] = key; A239070_hv[i] = val; A239070_hcnt++;
}

static uint64_t A239070_hget(uint64_t key) {
    if (!A239070_hcap) return 0;
    size_t i = (size_t)((key * 11400714819323198485ULL) >> 20) & (A239070_hcap - 1);
    while (A239070_hk[i]) {
        if (A239070_hk[i] == key) return A239070_hv[i];
        i = (i + 1) & (A239070_hcap - 1);
    }
    return 0;
}

static void A239070_step(void) {
    uint64_t p;
    if (A239070_np == 0) p = 2;
    else {
        for (;;) {
            uint64_t c = A239070_cand;
            bool ok = true;
            for (size_t i = 0; i < A239070_np; i++) {
                uint64_t q = A239070_pr[i];
                if (q * q > c) break;
                if (c % q == 0) { ok = false; break; }
            }
            A239070_cand += 2;
            if (ok) { p = c; break; }
        }
    }
    if (A239070_np == A239070_pcap) {
        A239070_pcap = A239070_pcap ? A239070_pcap * 2 : 64;
        A239070_pr = (uint64_t *)realloc(A239070_pr, A239070_pcap * sizeof(uint64_t));
    }
    A239070_pr[A239070_np++] = p;
    A239070_m++;
    A239070_sum += p;
    uint64_t t = A239070_sum, g = 1;
    for (size_t i = 0; i < A239070_np; i++) {
        uint64_t q = A239070_pr[i];
        if (q * q > t) break;
        if (t % q == 0) {
            g *= q;
            while (t % q == 0) t /= q;
        }
    }
    if (t > 1 && t <= p) g *= t;
    A239070_hput(g, A239070_m);
}

seq_t a(idx_t n) {
    uint64_t s = A239070_nth_sqfree(n);
    for (;;) {
        uint64_t r = A239070_hget(s);
        if (r) return (seq_t)r;
        A239070_step();
    }
}
