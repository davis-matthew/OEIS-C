/*
 * A067073  Records in A092419.
 * https://oeis.org/A067073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th record value of A092419 (least prime p with
 *            Kronecker (k|p) = -1, over nonsquares k). Computed by searching
 *            k in increasing order: brute force for small k, then a wheel mod
 *            8*3*5*...*19 plus a 64-bit-word sieve over the larger primes
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

#define R67073_NP 168
#define R67073_NPD 16
#define R67073_MAXREC 256

static uint64_t r67073_PR[R67073_NP];
static unsigned char r67073_AL[R67073_NP][1000]; /* 1 if residue is 0 or a QR */
static bool r67073_init = false;

static uint64_t r67073_recv[R67073_MAXREC];
static uint64_t r67073_reck[R67073_MAXREC];
static uint64_t r67073_nrec = 0;
static uint64_t r67073_scanned = 0; /* no k in (last record k, scanned] beats current record */

/* wheel data */
static bool r67073_wheel_ok = false;
static uint32_t *r67073_res = NULL;
static size_t r67073_W = 0;
static const uint64_t r67073_M = 38798760ULL; /* 8*3*5*7*11*13*17*19 */
static unsigned char *r67073_D = NULL;       /* [NPD][W]: (Minv*r) mod q */
static uint64_t r67073_Minv[R67073_NP];
static uint64_t *r67073_MASK[R67073_NP];

/* SHARED_HELPER: Jacobi symbol (a|n) for odd positive n */
static int r67073_jacobi(uint64_t a, uint64_t n) {
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t m = n & 7;
            if (m == 3 || m == 5) res = -res;
        }
        uint64_t t = a; a = n; n = t;
        if ((a & 3) == 3 && (n & 3) == 3) res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

static void r67073_setup(void) {
    if (r67073_init) return;
    int cnt = 0;
    for (int x = 2; x < 1000 && cnt < R67073_NP; x++) {
        bool pr = true;
        for (int d = 2; d * d <= x; d++) if (x % d == 0) { pr = false; break; }
        if (pr) r67073_PR[cnt++] = (uint64_t)x;
    }
    for (int i = 0; i < R67073_NP; i++) {
        int p = (int)r67073_PR[i];
        for (int r = 0; r < 1000; r++) r67073_AL[i][r] = 0;
        r67073_AL[i][0] = 1;
        for (int r = 1; r < p; r++) r67073_AL[i][(r * r) % p] = 1;
    }
    r67073_init = true;
}

/* least inert prime of k; 0 if k is a perfect square */
static uint64_t r67073_lpi(uint64_t k) {
    uint64_t m = k & 7;
    if (m == 3 || m == 5) return 2;
    for (int i = 1; i < R67073_NP; i++) {
        uint64_t p = r67073_PR[i];
        if (!r67073_AL[i][k % p]) return p;
    }
    uint64_t root = isqrt_u64(k);
    if (root * root == k) return 0;
    for (uint64_t p = 1009;; p += 2) {
        if (!is_prime_u64(p)) continue;
        uint64_t r = k % p;
        if (r && r67073_jacobi(r, p) == -1) return p;
    }
}

static void r67073_build_wheel(void) {
    if (r67073_wheel_ok) return;
    size_t cap = 544320;
    uint32_t *cur = (uint32_t *)malloc(cap * sizeof(uint32_t));
    uint32_t *nxt = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t m = 0;
    {
        static const unsigned init[6] = {0, 1, 2, 4, 6, 7};
        for (int i = 0; i < 6; i++) cur[m++] = init[i];
    }
    uint64_t M = 8;
    for (int w = 1; w <= 7; w++) { /* primes 3..19 */
        unsigned q = (unsigned)r67073_PR[w];
        size_t nm = 0;
        for (unsigned t = 0; t < q; t++) {
            for (size_t j = 0; j < m; j++) {
                uint64_t v = (uint64_t)cur[j] + (uint64_t)t * M;
                if (r67073_AL[w][v % q]) nxt[nm++] = (uint32_t)v;
            }
        }
        uint32_t *tmp = cur; cur = nxt; nxt = tmp;
        m = nm;
        M *= q;
    }
    free(nxt);
    r67073_res = cur;
    r67073_W = m;

    /* inverses of M and masks for primes >= 23 */
    for (int pi = 8; pi < R67073_NP; pi++) {
        uint64_t q = r67073_PR[pi];
        uint64_t mm = r67073_M % q, inv = 1;
        for (uint64_t x = 1; x < q; x++) if ((mm * x) % q == 1) { inv = x; break; }
        r67073_Minv[pi] = inv;
        unsigned char inB[1000];
        for (uint64_t u = 0; u < q; u++) inB[u] = r67073_AL[pi][(mm * u) % q];
        uint64_t *mk = (uint64_t *)malloc((size_t)q * sizeof(uint64_t));
        for (uint64_t o = 0; o < q; o++) {
            uint64_t w = 0;
            for (unsigned j = 0; j < 64; j++)
                if (inB[(o + j) % q]) w |= (1ULL << j);
            mk[o] = w;
        }
        r67073_MASK[pi] = mk;
    }
    r67073_D = (unsigned char *)malloc((size_t)R67073_NPD * m);
    for (int f = 0; f < R67073_NPD; f++) {
        int pi = 8 + f;
        uint64_t q = r67073_PR[pi], inv = r67073_Minv[pi];
        unsigned char *row = r67073_D + (size_t)f * m;
        for (size_t i = 0; i < m; i++)
            row[i] = (unsigned char)((inv * (r67073_res[i] % q)) % q);
    }
    r67073_wheel_ok = true;
}

/* smallest nonsquare k > kprev all of whose primes <= cur are non-inert */
static uint64_t r67073_find(uint64_t kprev, uint64_t cur) {
    r67073_build_wheel();
    int nf = 0;
    while (8 + nf < R67073_NP && r67073_PR[8 + nf] <= cur) nf++;
    uint64_t M = r67073_M;
    uint64_t T0 = (kprev + 1) / M;
    uint64_t tm[R67073_NP];
    for (;;) {
        for (int f = 0; f < nf; f++) tm[f] = T0 % r67073_PR[8 + f];
        uint64_t best = UINT64_MAX;
        for (size_t i = 0; i < r67073_W; i++) {
            uint64_t w = ~0ULL;
            uint32_t r = r67073_res[i];
            for (int f = 0; f < nf; f++) {
                int pi = 8 + f;
                uint64_t q = r67073_PR[pi];
                uint64_t d = (f < R67073_NPD) ? (uint64_t)r67073_D[(size_t)f * r67073_W + i]
                                              : (r67073_Minv[pi] * (r % q)) % q;
                uint64_t o = d + tm[f];
                if (o >= q) o -= q;
                w &= r67073_MASK[pi][o];
                if (!w) break;
            }
            if (!w) continue;
            for (unsigned j = 0; j < 64; j++) {
                if (!((w >> j) & 1ULL)) continue;
                uint64_t k = (uint64_t)r + M * (T0 + j);
                if (k <= kprev) continue;
                if (k >= best) break;
                uint64_t s = isqrt_u64(k);
                if (s * s == k) continue;
                best = k;
                break;
            }
        }
        if (best != UINT64_MAX) return best;
        T0 += 64;
    }
}

static void r67073_extend(void) {
    r67073_setup();
    if (r67073_nrec == 0) {
        uint64_t cur = 0;
        const uint64_t K1 = 100000;
        uint64_t k;
        for (k = 2; k <= K1 || cur < 23; k++) {
            uint64_t lp = r67073_lpi(k);
            if (lp == 0) continue;
            if (lp > cur && r67073_nrec < R67073_MAXREC) {
                cur = lp;
                r67073_recv[r67073_nrec] = lp;
                r67073_reck[r67073_nrec] = k;
                r67073_nrec++;
            }
        }
        r67073_scanned = k - 1;
        return;
    }
    uint64_t cur = r67073_recv[r67073_nrec - 1];
    uint64_t kprev = r67073_reck[r67073_nrec - 1];
    if (r67073_scanned > kprev) kprev = r67073_scanned;
    uint64_t k = r67073_find(kprev, cur);
    uint64_t lp = r67073_lpi(k);
    r67073_recv[r67073_nrec] = lp;
    r67073_reck[r67073_nrec] = k;
    r67073_nrec++;
    r67073_scanned = k;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > R67073_MAXREC) return (seq_t)0;
    while (r67073_nrec < (uint64_t)n) r67073_extend();
    return (seq_t)r67073_recv[n - 1];
}
