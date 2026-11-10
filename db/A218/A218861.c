/*
 * A218861  First position of A218860(n) in A066135.
 * https://oeis.org/A218861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k such that A066135(k) = A218860(n), the n-th
 *            distinct value of A066135 in order of first appearance. Computed
 *            by a search: evaluate A066135(k) for k = 1, 2, 3, ... (least m >
 *            1 with m | sigma_k(m)) and record each k where a value not seen
 *            before appears
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- smallest-prime-factor table (uint16, 0 = prime) ---------- */
#define A218861_SPF_CAP ((uint64_t)1 << 25)
static uint16_t *A218861_spf = NULL;
static uint64_t A218861_spf_lim = 0;

static uint32_t *A218861_small_primes = NULL;   /* primes < 65536 */
static size_t A218861_small_cnt = 0;

static void A218861_init_small_primes(void) {
    if (A218861_small_primes) return;
    uint32_t L = 65536;
    char *comp = (char *)calloc(L, 1);
    A218861_small_primes = (uint32_t *)malloc(L * sizeof(uint32_t));
    for (uint32_t i = 2; i < L; ++i) {
        if (!comp[i]) {
            A218861_small_primes[A218861_small_cnt++] = i;
            for (uint32_t j = i * i; j < L && i < 256; j += i) comp[j] = 1;
        }
    }
    free(comp);
}

static void A218861_ensure_spf(uint64_t need) {
    if (need < A218861_spf_lim || A218861_spf_lim >= A218861_SPF_CAP) return;
    uint64_t lim = A218861_spf_lim ? A218861_spf_lim : ((uint64_t)1 << 20);
    while (lim <= need && lim < A218861_SPF_CAP) lim <<= 1;
    if (lim > A218861_SPF_CAP) lim = A218861_SPF_CAP;
    free(A218861_spf);
    A218861_spf = (uint16_t *)calloc((size_t)lim, sizeof(uint16_t));
    for (uint64_t p = 2; p * p < lim; ++p) {
        if (A218861_spf[p] == 0) {
            for (uint64_t j = p * p; j < lim; j += p)
                if (A218861_spf[j] == 0) A218861_spf[j] = (uint16_t)p;
        }
    }
    A218861_spf_lim = lim;
}

static int A218861_factor(uint64_t m, uint64_t *pr, uint32_t *ex) {
    int k = 0;
    if (m < A218861_spf_lim) {
        while (m > 1) {
            uint64_t s = A218861_spf[m];
            if (s == 0) s = m;
            uint32_t e = 0;
            while (m % s == 0) { m /= s; ++e; }
            pr[k] = s; ex[k] = e; ++k;
        }
        return k;
    }
    for (size_t i = 0; i < A218861_small_cnt; ++i) {
        uint64_t p = A218861_small_primes[i];
        if (p * p > m) break;
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pr[k] = p; ex[k] = e; ++k;
        }
    }
    if (m > 1) {
        /* beyond 2^32 the remaining cofactor may still be composite */
        uint64_t p = 65537;
        while (p * p <= m) {
            if (m % p == 0) {
                uint32_t e = 0;
                while (m % p == 0) { m /= p; ++e; }
                pr[k] = p; ex[k] = e; ++k;
            }
            p += 2;
        }
        if (m > 1) { pr[k] = m; ex[k] = 1; ++k; }
    }
    return k;
}

static inline uint64_t A218861_mm(uint64_t a, uint64_t b, uint64_t Q) {
    if (Q <= 0xFFFFFFFFull) return (a * b) % Q;
    return (uint64_t)((unsigned __int128)a * b % Q);
}

static uint64_t A218861_pw(uint64_t b, uint64_t e, uint64_t Q) {
    uint64_t r = 1 % Q;
    b %= Q;
    while (e) {
        if (e & 1) r = A218861_mm(r, b, Q);
        b = A218861_mm(b, b, Q);
        e >>= 1;
    }
    return r;
}

/* does m divide sigma_n(m)?  checked prime power by prime power */
static bool A218861_works(uint64_t m, uint64_t n) {
    uint64_t pr[64];
    uint32_t ex[64];
    int k = A218861_factor(m, pr, ex);
    if (k == 1 && n >= ex[0]) return false;
    for (int i = k - 1; i >= 0; --i) {
        uint64_t q = pr[i];
        uint64_t Q = 1;
        for (uint32_t t = 0; t < ex[i]; ++t) Q *= q;
        uint64_t ph = (Q / q) * (q - 1);
        uint64_t r = n % ph;
        uint64_t prod = 1 % Q;
        for (int j = 0; j < k && prod != 0; ++j) {
            uint64_t x;
            if (j == i) {
                if (n >= ex[i]) continue;          /* factor is 1 mod Q */
                x = A218861_pw(q, n, Q);
            } else {
                x = A218861_pw(pr[j] % Q, r, Q);
            }
            uint64_t S = 1 % Q;
            for (uint32_t t = 1; t <= ex[j]; ++t)
                S = (A218861_mm(S, x, Q) + 1) % Q;
            prod = A218861_mm(prod, S, Q);
        }
        if (prod != 0) return false;
    }
    return true;
}

static uint64_t A218861_A066135(uint64_t n) {
    uint64_t m = 2;
    for (;;) {
        if (m >= A218861_spf_lim) A218861_ensure_spf(m);
        if (A218861_works(m, n)) return m;
        ++m;
    }
}

/* ---------- memoised state ---------- */
static uint64_t *A218861_vals = NULL;   /* distinct values seen */
static uint64_t *A218861_pos = NULL;    /* first positions */
static size_t A218861_cnt = 0, A218861_cap = 0;
static uint64_t A218861_done = 0;       /* A066135 evaluated for 1..done */

seq_t a(idx_t n) {
    A218861_init_small_primes();
    A218861_ensure_spf(2);
    while ((idx_t)A218861_cnt < n) {
        uint64_t k = A218861_done + 1;
        uint64_t v = A218861_A066135(k);
        A218861_done = k;
        bool seen = false;
        for (size_t i = 0; i < A218861_cnt; ++i)
            if (A218861_vals[i] == v) { seen = true; break; }
        if (!seen) {
            if (A218861_cnt == A218861_cap) {
                A218861_cap = A218861_cap ? 2 * A218861_cap : 64;
                A218861_vals = (uint64_t *)realloc(A218861_vals, A218861_cap * sizeof(uint64_t));
                A218861_pos = (uint64_t *)realloc(A218861_pos, A218861_cap * sizeof(uint64_t));
            }
            A218861_vals[A218861_cnt] = v;
            A218861_pos[A218861_cnt] = k;
            ++A218861_cnt;
        }
    }
    return (seq_t)A218861_pos[n - 1];
}
