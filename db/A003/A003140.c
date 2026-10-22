/*
 * A003140  Number of coprime chains with largest member prime(n).
 * https://oeis.org/A003140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search: split primes <= prime(n) into small (q^2 < p) and large;
 *            each large prime q < p joins one member q*m (m <= (p-1)/q, built
 *            from small primes); the small primes left over are split into
 *            members >= 2 with disjoint prime supports (a subset convolution
 *            over bitmasks of small primes); p = prime...
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   ccp_isprime, ccp_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ccp_isprime.h"
#include "../utils/ccp_nth_prime.h"

#define CCP_MAXK 14
#define CCP_SZ (1u << CCP_MAXK)

static seq_t ccp_c[CCP_SZ];
static seq_t ccp_F[CCP_SZ];
static seq_t ccp_G[CCP_SZ];
static seq_t ccp_H[CCP_SZ];
static uint64_t ccp_d[CCP_SZ];

static uint64_t *sm_val;
static unsigned *sm_mask;
static size_t sm_cnt, sm_cap;

static void ccp_gen(const uint64_t *S, int k, int idx, uint64_t m, unsigned mask, uint64_t lim) {
    if (idx == k) {
        if (sm_cnt == sm_cap) {
            sm_cap = sm_cap ? sm_cap * 2 : 64;
            sm_val = (uint64_t *)realloc(sm_val, sm_cap * sizeof(uint64_t));
            sm_mask = (unsigned *)realloc(sm_mask, sm_cap * sizeof(unsigned));
        }
        sm_val[sm_cnt] = m;
        sm_mask[sm_cnt] = mask;
        sm_cnt++;
        return;
    }
    ccp_gen(S, k, idx + 1, m, mask, lim);
    uint64_t r = S[idx];
    uint64_t v = m;
    while (v <= lim / r) {
        v *= r;
        ccp_gen(S, k, idx + 1, v, mask | (1u << idx), lim);
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t p = ccp_nth_prime(n);
    uint64_t lim = p - 1;

    uint64_t S[64];
    int k = 0;
    for (uint64_t r = 2; r * r < p; r++)
        if (ccp_isprime(r)) S[k++] = r;
    if (k > CCP_MAXK) return 0;

    unsigned full = (1u << k) - 1;
    size_t sz = (size_t)1 << k;

    sm_val = NULL; sm_mask = NULL; sm_cnt = 0; sm_cap = 0;
    ccp_gen(S, k, 0, 1, 0, lim);

    seq_t *c = ccp_c, *F = ccp_F, *G = ccp_G, *H = ccp_H;
    uint64_t *d = ccp_d;
    for (size_t i = 0; i < sz; i++) { c[i] = 0; F[i] = 0; G[i] = 0; H[i] = 0; }

    for (size_t i = 0; i < sm_cnt; i++)
        if (sm_val[i] >= 2) c[sm_mask[i]] += 1;

    F[0] = 1;
    for (unsigned mask = 1; mask <= full && mask != 0; mask++) {
        unsigned low = mask & (~mask + 1);
        unsigned rest = mask ^ low;
        seq_t acc = 0;
        unsigned t = rest;
        for (;;) {
            unsigned s = t | low;
            if (c[s] != 0) acc += c[s] * F[mask ^ s];
            if (t == 0) break;
            t = (t - 1) & rest;
        }
        F[mask] = acc;
    }

    G[0] = 1;
    for (uint64_t q = 2; q < p; q++) {
        if (q * q < p) continue;
        if (lim / q < 2) break;
        if (!ccp_isprime(q)) continue;
        uint64_t bound = lim / q;
        for (size_t i = 0; i < sz; i++) d[i] = 0;
        for (size_t i = 0; i < sm_cnt; i++)
            if (sm_val[i] <= bound) d[sm_mask[i]]++;
        for (size_t i = 0; i < sz; i++) H[i] = 0;
        for (unsigned u = 0; u <= full; u++) {
            if (G[u] != 0) {
                unsigned fr = full ^ u;
                unsigned t = fr;
                for (;;) {
                    if (d[t]) H[u | t] += G[u] * (seq_t)d[t];
                    if (t == 0) break;
                    t = (t - 1) & fr;
                }
            }
            if (u == full) break;
        }
        seq_t *tmp = G; G = H; H = tmp;
    }

    seq_t ans = 0;
    for (unsigned u = 0; u <= full; u++) {
        if (G[u] != 0) ans += G[u] * F[full ^ u];
        if (u == full) break;
    }

    free(sm_val); free(sm_mask);
    sm_val = NULL; sm_mask = NULL; sm_cnt = 0; sm_cap = 0;
    return ans;
}
