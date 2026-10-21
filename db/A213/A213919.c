/*
 * A213919  Triangle read by rows: T(n,m) = (n/k)^(k-1), where k is the m-th
 *          divisor of n, 1 <= m <= tau(n).
 * https://oeis.org/A213919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (N / k)^(k-1), where N is the row with cumulative divisor
 *            count >= n and k is the m-th divisor of N (m = n -
 *            cumulative_{<N} tau(i))
 * Range:     u8 n <= 56, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses incremental sieve and divisor generation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static size_t psize = 0;
static size_t pcap  = 0;
static uint32_t *primes = NULL;

static void ensure_primes_up_to(uint64_t limit)
{
    if (psize == 0) {
        primes = (uint32_t*)malloc(64 * sizeof(uint32_t));
        pcap = 64;
        primes[psize++] = 2;
    }
    uint32_t start = primes[psize-1] + 1;
    for (uint32_t cand = start; (uint64_t)cand * cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < psize && (uint64_t)primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (psize == pcap) {
                pcap = pcap * 2;
                primes = (uint32_t*)realloc(primes, pcap * sizeof(uint32_t));
            }
            primes[psize++] = cand;
        }
    }
}

static idx_t compute_tau(idx_t n)
{
    uint64_t temp = (uint64_t)n;
    ensure_primes_up_to(n);
    idx_t result = 1;
    for (size_t i = 0; i < psize && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            idx_t e = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++e;
            }
            result *= (e + 1);
        }
    }
    if (temp > 1) result *= 2;
    return result;
}

static void gen_divs_rec(uint64_t curr, size_t idx,
                         const uint32_t *pfs, const uint32_t *exps,
                         size_t pf_cnt, uint64_t *out, size_t *outcnt)
{
    if (idx == pf_cnt) {
        out[(*outcnt)++] = curr;
        return;
    }
    uint64_t val = curr;
    for (uint32_t i = 0; i <= exps[idx]; ++i) {
        gen_divs_rec(val, idx + 1, pfs, exps, pf_cnt, out, outcnt);
        val *= pfs[idx];
    }
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t*)a;
    uint64_t y = *(const uint64_t*)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static idx_t *cum = NULL;
    static idx_t *tau = NULL;
    static size_t sz  = 0;
    static size_t cap = 0;

    if (!cum) {                       /* one-time allocation */
        cap = 128;
        cum = (idx_t*)calloc(cap + 1, sizeof(idx_t));
        tau = (idx_t*)calloc(cap + 1, sizeof(idx_t));
        cum[0] = 0; tau[0] = 0;
    }

    /* extend cumulative sums until they cover the requested index */
    while (n > cum[sz]) {
        ++sz;
        if (sz > cap) {
            cap <<= 1; /* double capacity */
            cum = (idx_t*)realloc(cum, (cap + 1) * sizeof(idx_t));
            tau = (idx_t*)realloc(tau, (cap + 1) * sizeof(idx_t));
        }
        tau[sz] = compute_tau((idx_t)sz);
        cum[sz] = cum[sz - 1] + tau[sz];
    }

    /* locate the row (N) and the position (m) within the row */
    idx_t low  = 1;
    idx_t high = sz;
    idx_t row  = sz;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cum[mid - 1] < n && n <= cum[mid]) {
            row = mid;
            break;
        }
        if (n > cum[mid])
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t prev = cum[row - 1];
    idx_t m    = n - prev;          /* 1‑based index inside the row */

    /* factor the row number to obtain its divisors */
    ensure_primes_up_to((uint64_t)row);
    uint32_t pf[32], exp[32];
    size_t pfcnt = 0;
    uint64_t temp = row;
    for (size_t i = 0; i < psize && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            uint32_t e = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++e;
            }
            pf[pfcnt]   = primes[i];
            exp[pfcnt++] = e;
        }
    }
    if (temp > 1) {
        pf[pfcnt]   = (uint32_t)temp;
        exp[pfcnt++] = 1;
    }

    /* enumerate all divisors of the row */
    uint64_t divs[256];
    size_t divcnt = 0;
    gen_divs_rec(1, 0, pf, exp, pfcnt, divs, &divcnt);

    /* sort the divisors */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    uint64_t k = divs[m - 1];
    seq_t  base = (seq_t)(row / k);
    seq_t  res  = big_pow(base, (seq_t)(k - 1));
    return res;
}
