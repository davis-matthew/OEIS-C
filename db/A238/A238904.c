/*
 * A238904  Smallest k such that 2^k + (2n+1) and (2n+1)*2^k + 1 are both
 *          prime, k <= n, or -1 if no such k exists.
 * https://oeis.org/A238904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   search k = 0..n for the smallest k with 2^k+(2n+1) and
 *            (2n+1)*2^k+1 both prime; a(n) = -1 if none. The search uses a
 *            small-prime sieve and Miller-Rabin tests on multi-limb integers,
 *            so it does not depend on seq_t
 * Range:     i8 n <= 688, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 95 terms through n = 986 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

#define SIEVE_LIM 20000
static unsigned sp[2400];
static int nsp = 0;

static void init_sieve(void)
{
    if (nsp) return;
    static unsigned char comp[SIEVE_LIM + 1];
    int i, j;
    for (i = 2; i <= SIEVE_LIM; i++) {
        if (!comp[i]) {
            if (i > 2) sp[nsp++] = (unsigned)i;
            for (j = i * i; j <= SIEVE_LIM; j += i) comp[j] = 1;
        }
    }
}

static int bcmp_(const uint64_t *a, const uint64_t *b, int L)
{
    int i;
    for (i = L - 1; i >= 0; i--)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

static void bsub_(uint64_t *a, const uint64_t *b, int L)
{
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint64_t ai = a[i], bi = b[i];
        uint64_t d = ai - bi - borrow;
        borrow = (ai < bi) || (ai == bi && borrow) ? 1 : 0;
        a[i] = d;
    }
}

static void bdouble_(uint64_t *x, const uint64_t *N, int L)
{
    uint64_t carry = x[L - 1] >> 63;
    int i;
    for (i = L - 1; i > 0; i--) x[i] = (x[i] << 1) | (x[i - 1] >> 63);
    x[0] <<= 1;
    if (carry || bcmp_(x, N, L) >= 0) bsub_(x, N, L);
}

static void baddmod_(uint64_t *x, const uint64_t *y, const uint64_t *N, int L)
{
    unsigned __int128 c = 0;
    int i;
    for (i = 0; i < L; i++) {
        c += (unsigned __int128)x[i] + y[i];
        x[i] = (uint64_t)c;
        c >>= 64;
    }
    if (c || bcmp_(x, N, L) >= 0) bsub_(x, N, L);
}

static void mmul_(uint64_t *r, const uint64_t *a, const uint64_t *b,
                  const uint64_t *N, uint64_t ninv, int L, uint64_t *t)
{
    int i, j;
    for (i = 0; i < L + 2; i++) t[i] = 0;
    for (i = 0; i < L; i++) {
        unsigned __int128 c = 0;
        uint64_t bi = b[i];
        for (j = 0; j < L; j++) {
            c += (unsigned __int128)a[j] * bi + t[j];
            t[j] = (uint64_t)c;
            c >>= 64;
        }
        c += t[L];
        t[L] = (uint64_t)c;
        t[L + 1] = (uint64_t)(c >> 64);
        uint64_t mq = t[0] * ninv;
        c = (unsigned __int128)mq * N[0] + t[0];
        c >>= 64;
        for (j = 1; j < L; j++) {
            c += (unsigned __int128)mq * N[j] + t[j];
            t[j - 1] = (uint64_t)c;
            c >>= 64;
        }
        c += t[L];
        t[L - 1] = (uint64_t)c;
        t[L] = t[L + 1] + (uint64_t)(c >> 64);
        t[L + 1] = 0;
    }
    if (t[L] || bcmp_(t, N, L) >= 0) bsub_(t, N, L);
    for (i = 0; i < L; i++) r[i] = t[i];
}

/* probable-prime test of odd N (L limbs, top limb nonzero, N > 2^63) */
static bool big_prp(const uint64_t *N, int L, uint64_t *buf)
{
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    uint64_t *one = buf, *mone = buf + L, *x = buf + 2 * L, *aM = buf + 3 * L, *t = buf + 4 * L;
    int i, bi, hb, s, bit;
    uint64_t inv = N[0];
    for (i = 0; i < 6; i++) inv *= 2 - N[0] * inv;
    uint64_t ninv = (uint64_t)0 - inv;

    for (i = 0; i < L; i++) one[i] = 0;
    one[0] = 1;
    for (i = 0; i < 64 * L; i++) bdouble_(one, N, L);
    for (i = 0; i < L; i++) mone[i] = N[i];
    bsub_(mone, one, L);

    hb = 64 * (L - 1) + 63;
    while (!((N[hb / 64] >> (hb % 64)) & 1)) hb--;
    s = 1;
    while (!((N[s / 64] >> (s % 64)) & 1)) s++;

    for (bi = 0; bi < (int)(sizeof(bases) / sizeof(bases[0])); bi++) {
        uint64_t b = bases[bi], c;
        for (i = 0; i < L; i++) aM[i] = 0;
        for (c = 0; c < b; c++) baddmod_(aM, one, N, L);
        for (i = 0; i < L; i++) x[i] = aM[i];
        for (bit = hb - 1; bit >= s; bit--) {
            mmul_(x, x, x, N, ninv, L, t);
            if ((N[bit / 64] >> (bit % 64)) & 1) mmul_(x, x, aM, N, ninv, L, t);
        }
        if (bcmp_(x, one, L) == 0 || bcmp_(x, mone, L) == 0) continue;
        bool pass = false;
        int r;
        for (r = 1; r < s; r++) {
            mmul_(x, x, x, N, ninv, L, t);
            if (bcmp_(x, mone, L) == 0) { pass = true; break; }
            if (bcmp_(x, one, L) == 0) break;
        }
        if (!pass) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    init_sieve();
    uint64_t m = 2 * (uint64_t)n + 1;
    int mbits = 0;
    { uint64_t tt = m; while (tt) { mbits++; tt >>= 1; } }

    unsigned *r2 = (unsigned *)malloc(sizeof(unsigned) * (size_t)nsp);
    unsigned *mm = (unsigned *)malloc(sizeof(unsigned) * (size_t)nsp);
    int i;
    for (i = 0; i < nsp; i++) { r2[i] = 1 % sp[i]; mm[i] = (unsigned)(m % sp[i]); }

    int Lmax = (int)(((uint64_t)n + (uint64_t)mbits + 2) / 64) + 3;
    uint64_t *num = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)Lmax);
    uint64_t *buf = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(5 * Lmax + 4));

    long long res = -1;
    uint64_t k;
    for (k = 0; k <= (uint64_t)n; k++) {
        if (k + (uint64_t)mbits <= 63) {
            uint64_t v1 = ((uint64_t)1 << k) + m;
            uint64_t v2 = (m << k) + 1;
            if (is_prime_u64(v1) && is_prime_u64(v2)) { res = (long long)k; break; }
        } else {
            bool ok = true;
            for (i = 0; i < nsp; i++) {
                unsigned q = sp[i];
                if ((r2[i] + mm[i]) % q == 0) { ok = false; break; }
                if (((uint64_t)mm[i] * r2[i] + 1) % q == 0) { ok = false; break; }
            }
            if (ok) {
                int L = (int)((k + (uint64_t)mbits + 1) / 64) + 1;
                int j;
                /* v1 = 2^k + m */
                for (j = 0; j < L; j++) num[j] = 0;
                num[k / 64] |= (uint64_t)1 << (k % 64);
                {
                    unsigned __int128 c = (unsigned __int128)num[0] + m;
                    num[0] = (uint64_t)c; c >>= 64;
                    for (j = 1; j < L && c; j++) { c += num[j]; num[j] = (uint64_t)c; c >>= 64; }
                }
                int L1 = L;
                while (L1 > 1 && num[L1 - 1] == 0) L1--;
                if (big_prp(num, L1, buf)) {
                    /* v2 = m*2^k + 1 */
                    for (j = 0; j < L; j++) num[j] = 0;
                    {
                        int w = (int)(k / 64), sh = (int)(k % 64);
                        num[w] |= m << sh;
                        if (sh) num[w + 1] |= m >> (64 - sh);
                    }
                    num[0] |= 1;
                    int L2 = L;
                    while (L2 > 1 && num[L2 - 1] == 0) L2--;
                    if (big_prp(num, L2, buf)) { res = (long long)k; break; }
                }
            }
        }
        for (i = 0; i < nsp; i++) r2[i] = (r2[i] * 2) % sp[i];
    }

    free(r2); free(mm); free(num); free(buf);
    if (res < 0) return (seq_t)(-1);
    return (seq_t)res;
}
