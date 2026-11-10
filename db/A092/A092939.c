/*
 * A092939  Index of the first occurrence of prime(n) in A092938.
 * https://oeis.org/A092939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not closed form: walk primes r = prime(k) in increasing
 *            k (segmented sieve); A092938(k) = least prime p with 2r - p
 *            prime; a(n) = first k with A092938(k) = prime(n)
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 149, u64 n <= 149,
 *            u128 n <= 149, big any n
 * Validated: standard, pass, 65 terms through n = 139 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

static uint32_t *A092939_sp = NULL;   /* small primes */
static size_t A092939_spn = 0;
static uint64_t A092939_splim = 0;

static void A092939_sp_ensure(uint64_t lim)
{
    if (lim <= A092939_splim) return;
    uint64_t nl = A092939_splim * 2;
    if (nl < lim) nl = lim;
    if (nl < 65536) nl = 65536;
    uint8_t *c = (uint8_t *)malloc((size_t)nl + 1);
    if (!c) abort();
    memset(c, 1, (size_t)nl + 1);
    c[0] = 0; c[1] = 0;
    for (uint64_t i = 2; i * i <= nl; i++)
        if (c[i]) for (uint64_t j = i * i; j <= nl; j += i) c[j] = 0;
    size_t cnt = 0;
    for (uint64_t i = 2; i <= nl; i++) if (c[i]) cnt++;
    uint32_t *np = (uint32_t *)realloc(A092939_sp, cnt * sizeof(uint32_t));
    if (!np) abort();
    A092939_sp = np;
    cnt = 0;
    for (uint64_t i = 2; i <= nl; i++) if (c[i]) A092939_sp[cnt++] = (uint32_t)i;
    A092939_spn = cnt;
    A092939_splim = nl;
    free(c);
}

static void A092939_sieve_range(uint64_t lo, size_t len, uint8_t *isp)
{
    uint64_t hi = lo + len;
    memset(isp, 1, len);
    for (uint64_t v = lo; v < 2 && v < hi; v++) isp[v - lo] = 0;
    uint64_t s = 1;
    while (s * s < hi) s++;
    A092939_sp_ensure(s + 1);
    for (size_t i = 0; i < A092939_spn; i++) {
        uint64_t q = A092939_sp[i];
        if (q * q >= hi) break;
        uint64_t st = ((lo + q - 1) / q) * q;
        if (st < q * q) st = q * q;
        for (uint64_t j = st; j < hi; j += q) isp[j - lo] = 0;
    }
}

#define A092939_S ((size_t)1 << 18)
#define A092939_W ((uint64_t)8192)

static uint64_t *A092939_fi = NULL;
static size_t A092939_fcap = 0;
static uint64_t A092939_L = 2;
static uint64_t A092939_k = 0;
static uint8_t *A092939_rw = NULL;
static uint8_t *A092939_ww = NULL;

static void A092939_record(size_t idx, uint64_t k)
{
    if (idx > A092939_fcap) {
        size_t nc = A092939_fcap ? A092939_fcap : 256;
        while (nc < idx) nc <<= 1;
        uint64_t *nf = (uint64_t *)realloc(A092939_fi, nc * sizeof(uint64_t));
        if (!nf) abort();
        memset(nf + A092939_fcap, 0, (nc - A092939_fcap) * sizeof(uint64_t));
        A092939_fi = nf;
        A092939_fcap = nc;
    }
    if (A092939_fi[idx - 1] == 0) A092939_fi[idx - 1] = k;
}

static void A092939_segment(void)
{
    size_t S = A092939_S;
    uint64_t L = A092939_L;
    if (!A092939_rw) {
        A092939_rw = (uint8_t *)malloc(S);
        A092939_ww = (uint8_t *)malloc(2 * S + (size_t)A092939_W);
        if (!A092939_rw || !A092939_ww) abort();
    }
    A092939_sieve_range(L, S, A092939_rw);
    uint64_t wlo = (2 * L > A092939_W) ? 2 * L - A092939_W : 0;
    size_t wlen = (size_t)(2 * L + 2 * S - wlo);
    A092939_sieve_range(wlo, wlen, A092939_ww);
    for (size_t t = 0; t < S; t++) {
        if (!A092939_rw[t]) continue;
        uint64_t r = L + t;
        A092939_k++;
        uint64_t two = 2 * r;
        size_t i = 0;
        for (;;) {
            if (i >= A092939_spn) A092939_sp_ensure(A092939_splim * 2);
            uint64_t p = A092939_sp[i];
            uint64_t q = two - p;
            bool pr = (q >= wlo) ? (A092939_ww[q - wlo] != 0) : is_prime_u64(q);
            if (pr) { A092939_record(i + 1, A092939_k); break; }
            i++;
        }
    }
    A092939_L = L + S;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;
    while (N > A092939_fcap || A092939_fi[N - 1] == 0)
        A092939_segment();
    return (seq_t)A092939_fi[N - 1];
}
