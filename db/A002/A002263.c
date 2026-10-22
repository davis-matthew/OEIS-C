/*
 * A002263  Numbers k such that 25*4^k + 1 is prime.
 * https://oeis.org/A002263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k >= 1: k is a term iff 25*4^k + 1 is prime (trial
 *            division for small k; for larger k, sieve by small primes, then
 *            Proth's test with base 3)
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   s2263_cmp, s2263_small_prime, s2263_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/s2263_cmp.h"
#include "../utils/s2263_small_prime.h"
#include "../utils/s2263_sub.h"

#define S2263_LIMIT 1000000u

typedef unsigned __int128 s2263_u128;

static uint32_t *s2263_primes = NULL;
static uint32_t *s2263_res = NULL;
static size_t s2263_np = 0;
static uint64_t s2263_k = 0;
static uint64_t *s2263_terms = NULL;
static size_t s2263_cnt = 0, s2263_cap = 0;

static void s2263_init(void) {
    char *comp = (char *)calloc(S2263_LIMIT + 1, 1);
    size_t cap = 100000;
    s2263_primes = (uint32_t *)malloc(sizeof(uint32_t) * cap);
    for (uint64_t i = 2; i <= S2263_LIMIT; i++) {
        if (comp[i]) continue;
        if (i > 5) {
            if (s2263_np == cap) {
                cap *= 2;
                s2263_primes = (uint32_t *)realloc(s2263_primes, sizeof(uint32_t) * cap);
            }
            s2263_primes[s2263_np++] = (uint32_t)i;
        }
        for (uint64_t j = i * i; j <= S2263_LIMIT; j += i) comp[j] = 1;
    }
    free(comp);
    s2263_res = (uint32_t *)malloc(sizeof(uint32_t) * (s2263_np + 1));
    for (size_t i = 0; i < s2263_np; i++) s2263_res[i] = 25u % s2263_primes[i];
}

static void s2263_add(uint64_t *x, const uint64_t *y, size_t L) {
    s2263_u128 c = 0;
    for (size_t i = 0; i < L; i++) {
        c += (s2263_u128)x[i] + y[i];
        x[i] = (uint64_t)c;
        c >>= 64;
    }
}

/* N = 25*2^m + 1: Proth test with base 3, i.e. 3^((N-1)/2) == -1 mod N */
static bool s2263_proth(uint64_t m) {
    size_t L = (size_t)((m + 7) / 64 + 1);
    size_t L2 = 2 * L;
    uint64_t *N = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *x = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *P = (uint64_t *)calloc(L2, sizeof(uint64_t));
    uint64_t *H = (uint64_t *)calloc(L2, sizeof(uint64_t));
    size_t w = (size_t)(m / 64);
    unsigned b = (unsigned)(m % 64);
    N[w] |= 25ull << b;
    if (b > 59) N[w + 1] |= 25ull >> (64 - b);
    N[0] |= 1;
    x[0] = 847288609443ull; /* 3^25 */
    if (L == 1) x[0] %= N[0];

    for (uint64_t it = 1; it < m; it++) {
        memset(P, 0, L2 * sizeof(uint64_t));
        for (size_t i = 0; i < L; i++) {
            if (!x[i]) continue;
            s2263_u128 c = 0;
            for (size_t j = i + 1; j < L; j++) {
                c += (s2263_u128)x[i] * x[j] + P[i + j];
                P[i + j] = (uint64_t)c;
                c >>= 64;
            }
            P[i + L] = (uint64_t)c;
        }
        for (size_t i = L2; i-- > 1;) P[i] = (P[i] << 1) | (P[i - 1] >> 63);
        P[0] <<= 1;
        {
            s2263_u128 c = 0;
            for (size_t i = 0; i < L; i++) {
                s2263_u128 sq = (s2263_u128)x[i] * x[i];
                c += (s2263_u128)P[2 * i] + (uint64_t)sq;
                P[2 * i] = (uint64_t)c;
                c >>= 64;
                c += (s2263_u128)P[2 * i + 1] + (uint64_t)(sq >> 64);
                P[2 * i + 1] = (uint64_t)c;
                c >>= 64;
            }
        }
        /* H = P >> m */
        for (size_t t = 0; t < L2; t++) {
            size_t src = t + w;
            uint64_t v = 0;
            if (src < L2) {
                v = P[src] >> b;
                if (b && src + 1 < L2) v |= P[src + 1] << (64 - b);
            }
            H[t] = v;
        }
        /* x = P mod 2^m */
        for (size_t t = 0; t < L; t++) {
            if (t < w) x[t] = P[t];
            else if (t == w) x[t] = b ? (P[t] & ((1ull << b) - 1)) : 0;
            else x[t] = 0;
        }
        /* H = 25*Q + R, using 32-bit halves so divisions stay in 64 bits */
        uint64_t R = 0;
        for (size_t t = L2; t-- > 0;) {
            uint64_t hi = H[t] >> 32, lo = H[t] & 0xffffffffull;
            uint64_t cur = (R << 32) | hi;
            uint64_t qh = cur / 25;
            R = cur % 25;
            cur = (R << 32) | lo;
            uint64_t ql = cur / 25;
            R = cur % 25;
            H[t] = (qh << 32) | ql;
        }
        /* x = R*2^m + Lpart + 2N - Q  (since 25*2^m == -1 mod N) */
        x[w] += R << b;
        if (b > 59) x[w + 1] += R >> (64 - b);
        s2263_add(x, N, L);
        s2263_add(x, N, L);
        s2263_sub(x, H, L);
        while (s2263_cmp(x, N, L) >= 0) s2263_sub(x, N, L);
    }
    /* check x == N - 1 */
    N[0] -= 1;
    bool ok = s2263_cmp(x, N, L) == 0;
    free(N);
    free(x);
    free(P);
    free(H);
    return ok;
}

static void s2263_advance(void) {
    s2263_k++;
    bool composite = false;
    for (size_t i = 0; i < s2263_np; i++) {
        uint32_t p = s2263_primes[i];
        uint32_t r = s2263_res[i] * 4u;
        while (r >= p) r -= p;
        s2263_res[i] = r;
        if (r == p - 1) composite = true;
    }
    bool prime;
    if (s2263_k <= 10) {
        uint64_t N = 25ull * (1ull << (2 * s2263_k)) + 1;
        prime = s2263_small_prime(N);
    } else {
        prime = !composite && s2263_proth(2 * s2263_k);
    }
    if (prime) {
        if (s2263_cnt == s2263_cap) {
            s2263_cap = s2263_cap ? 2 * s2263_cap : 64;
            s2263_terms = (uint64_t *)realloc(s2263_terms, sizeof(uint64_t) * s2263_cap);
        }
        s2263_terms[s2263_cnt++] = s2263_k;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!s2263_primes) s2263_init();
    while ((idx_t)s2263_cnt < n) s2263_advance();
    return (seq_t)s2263_terms[n - 1];
}
