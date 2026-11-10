/*
 * A051610  Monotonic subsequence of A051169.
 * https://oeis.org/A051610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m for the n-th m where A002373(m) sets a new record, i.e.
 *            A002373(m) > A002373(k) for all 3 <= k < m. Computed by a
 *            search: a segmented sieve of odd primes is shifted and ORed word
 *            by word to find every even 2m that the current small primes do
 *            not cover; only those m are checked one by one
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

#define A051610_W 4096u                 /* look-back in odd-index units (p <= 8191) */
#define A051610_L (1u << 20)            /* m-values per segment */
#define A051610_T (A051610_L + A051610_W)
#define A051610_TW (A051610_T / 64u)
#define A051610_PAT 15015u              /* 3*5*7*11*13 */
#define A051610_SPL (1u << 20)          /* sieving primes limit */
#define A051610_MAXCP 1100

static uint64_t A051610_recs[256];
static int A051610_cnt = 0;
static uint64_t A051610_best = 0;
static uint64_t A051610_M0 = 0;
static bool A051610_ready = false;

static uint64_t *A051610_seg = NULL;
static uint64_t *A051610_pat = NULL;
static uint32_t *A051610_sp = NULL;
static size_t A051610_nsp = 0;
static uint32_t A051610_cp[A051610_MAXCP];
static uint32_t A051610_cw[A051610_MAXCP];
static uint32_t A051610_cr[A051610_MAXCP];
static int A051610_ncp = 0;

static void A051610_record(uint64_t m, uint64_t p) {
    if (A051610_cnt < 256) A051610_recs[A051610_cnt++] = m;
    A051610_best = p;
}

static void A051610_init(void) {
    uint32_t i;
    uint64_t j;
    uint8_t *comp = (uint8_t *)calloc(A051610_SPL + 1, 1);
    A051610_sp = (uint32_t *)malloc(sizeof(uint32_t) * 90000);
    A051610_nsp = 0;
    A051610_ncp = 0;
    for (i = 2; i <= A051610_SPL; i++) {
        if (comp[i]) continue;
        if (i >= 17) A051610_sp[A051610_nsp++] = i;
        if (i >= 3 && i <= 8191 && A051610_ncp < A051610_MAXCP) {
            uint32_t s = (i + 1) / 2;
            uint32_t off = A051610_W - s;
            A051610_cp[A051610_ncp] = i;
            A051610_cw[A051610_ncp] = off >> 6;
            A051610_cr[A051610_ncp] = off & 63u;
            A051610_ncp++;
        }
        for (j = (uint64_t)i * i; j <= A051610_SPL; j += i) comp[j] = 1;
    }
    free(comp);

    /* pattern over odd indices (bit b <-> value 2b+1), period 64*15015 bits */
    A051610_pat = (uint64_t *)malloc(sizeof(uint64_t) * A051610_PAT);
    for (i = 0; i < A051610_PAT; i++) A051610_pat[i] = ~0ULL;
    {
        static const uint32_t sm[5] = {3, 5, 7, 11, 13};
        int t;
        uint64_t nbits = (uint64_t)A051610_PAT * 64u;
        for (t = 0; t < 5; t++) {
            uint64_t q = sm[t];
            uint64_t b;
            for (b = (q - 1) / 2; b < nbits; b += q)
                A051610_pat[b >> 6] &= ~(1ULL << (b & 63));
        }
    }
    A051610_seg = (uint64_t *)malloc(sizeof(uint64_t) * A051610_TW);

    /* brute force small m */
    {
        uint64_t m;
        for (m = 3; m < A051610_W; m++) {
            uint64_t target = 2 * m, p = 3;
            while (p < target && !(is_prime_u64(p) && is_prime_u64(target - p))) p += 2;
            if (p > A051610_best) A051610_record(m, p);
        }
    }
    A051610_M0 = A051610_W;
    A051610_ready = true;
}

static inline bool A051610_bit(uint64_t t) {
    return (A051610_seg[t >> 6] >> (t & 63)) & 1u;
}

/* smallest p (from coverage prime index k on) with 2m-p prime; rel = m - M0 */
static uint64_t A051610_minp(uint64_t m, uint64_t rel, int k) {
    int kk;
    uint64_t p;
    for (kk = k; kk < A051610_ncp; kk++) {
        uint64_t s = (A051610_cp[kk] + 1) / 2;
        if (A051610_bit(A051610_W + rel - s)) return A051610_cp[kk];
    }
    for (p = 8193; p < 2 * m; p += 2)
        if (is_prime_u64(p) && is_prime_u64(2 * m - p)) return p;
    return 0;
}

static void A051610_segment(void) {
    uint64_t *seg = A051610_seg;
    uint64_t M0 = A051610_M0;
    uint64_t B = M0 - A051610_W;
    uint64_t bw = B / 64u;
    uint64_t r0 = bw % A051610_PAT;
    uint64_t w, hiVal;
    size_t qi;
    uint32_t j;

    for (w = 0; w < A051610_TW; w++) {
        seg[w] = A051610_pat[r0];
        if (++r0 == A051610_PAT) r0 = 0;
    }
    if (B == 0) {
        seg[0] &= ~1ULL;                                   /* 1 is not prime */
        seg[0] |= (1ULL << 1) | (1ULL << 2) | (1ULL << 3) | (1ULL << 5) | (1ULL << 6);
    }
    hiVal = 2 * (B + A051610_T - 1) + 1;
    for (qi = 0; qi < A051610_nsp; qi++) {
        uint64_t q = A051610_sp[qi];
        uint64_t h, i, sq, t;
        if (q * q > hiVal) break;
        h = (q - 1) / 2;
        i = B + ((h + q - (B % q)) % q);
        sq = (q * q - 1) / 2;
        if (i < sq) i = sq;
        for (t = i - B; t < A051610_T; t += q)
            seg[t >> 6] &= ~(1ULL << (t & 63));
    }

    for (j = 0; j < A051610_L / 64u; j++) {
        uint64_t cov = 0;
        int k = 0;
        while (k < A051610_ncp && A051610_cp[k] <= A051610_best) {
            uint32_t bwi = j + A051610_cw[k];
            uint32_t r = A051610_cr[k];
            uint64_t v = seg[bwi] >> r;
            if (r) v |= seg[bwi + 1] << (64 - r);
            cov |= v;
            k++;
            if (cov == ~0ULL) break;
        }
        if (cov != ~0ULL) {
            uint64_t un = ~cov;
            while (un) {
                int bit = __builtin_ctzll(un);
                uint64_t rel, m, p;
                un &= un - 1;
                rel = (uint64_t)j * 64u + (uint64_t)bit;
                m = M0 + rel;
                p = A051610_minp(m, rel, k);
                if (p > A051610_best) A051610_record(m, p);
            }
        }
    }
    A051610_M0 = M0 + A051610_L;
}

seq_t a(idx_t n) {
    if (!A051610_ready) A051610_init();
    if (n < 1 || n > 256) return 0;
    while ((uint64_t)A051610_cnt < (uint64_t)n) A051610_segment();
    return (seq_t)A051610_recs[n - 1];
}
