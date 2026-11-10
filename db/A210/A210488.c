/*
 * A210488  a(n) is the index of A210487 when the n-th prime appears for the
 *          first time.
 * https://oeis.org/A210488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = least m >= 2 with A210487(m) = prime(n), where A210487(m)
 *            = min_{1<=k<m} GPF(prime(m)^2 - prime(k)^2); found by
 *            incremental search with memoization
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/largest_prime_factor.h"

static uint32_t *A210488_pr = NULL;
static size_t A210488_psz = 0, A210488_pcap = 0;
static uint32_t A210488_cand = 3;

static uint32_t A210488_prime(size_t k)
{
    if (A210488_pcap == 0) {
        A210488_pcap = 1024;
        A210488_pr = (uint32_t *)malloc(A210488_pcap * sizeof(uint32_t));
        if (!A210488_pr) abort();
        A210488_pr[A210488_psz++] = 2;
    }
    while (A210488_psz < k) {
        uint32_t p = A210488_cand;
        bool ok = true;
        for (size_t i = 0; i < A210488_psz; ++i) {
            uint64_t q = A210488_pr[i];
            if (q * q > p) break;
            if (p % q == 0) { ok = false; break; }
        }
        A210488_cand += 2;
        if (!ok) continue;
        if (A210488_psz == A210488_pcap) {
            A210488_pcap <<= 1;
            A210488_pr = (uint32_t *)realloc(A210488_pr, A210488_pcap * sizeof(uint32_t));
            if (!A210488_pr) abort();
        }
        A210488_pr[A210488_psz++] = p;
    }
    return A210488_pr[k - 1];
}

/* A210487 values, index m stored at vals[m]; vals[0], vals[1] unused */
static uint64_t *A210488_vals = NULL;
static size_t A210488_vcnt = 0, A210488_vcap = 0;

static uint64_t A210488_compute(size_t m)
{
    uint64_t pn = A210488_prime(m);
    uint64_t best = UINT64_MAX;
    for (size_t k = 1; k < m; ++k) {
        uint64_t pk = A210488_prime(k);
        uint64_t g1 = largest_prime_factor(pn - pk);
        uint64_t g2 = largest_prime_factor(pn + pk);
        uint64_t g = g1 > g2 ? g1 : g2;
        if (g < best) {
            best = g;
            if (best == 2) break;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    uint64_t target = A210488_prime((size_t)n);
    if (A210488_vcap == 0) {
        A210488_vcap = 1024;
        A210488_vals = (uint64_t *)malloc(A210488_vcap * sizeof(uint64_t));
        if (!A210488_vals) abort();
        A210488_vals[0] = 0;
        A210488_vals[1] = 0;
        A210488_vcnt = 2;
    }
    for (size_t m = 2; m < A210488_vcnt; ++m)
        if (A210488_vals[m] == target) return (seq_t)m;
    for (;;) {
        size_t m = A210488_vcnt;
        if (m == A210488_vcap) {
            A210488_vcap <<= 1;
            A210488_vals = (uint64_t *)realloc(A210488_vals, A210488_vcap * sizeof(uint64_t));
            if (!A210488_vals) abort();
        }
        uint64_t v = A210488_compute(m);
        A210488_vals[m] = v;
        A210488_vcnt = m + 1;
        if (v == target) return (seq_t)m;
    }
}
