/*
 * A074645  Numbers k such that A000010(k) divides A074639(k).
 * https://oeis.org/A074645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff A000010(k) divides A074639(k), where
 *            A074639(k) = (Sum_{h:(h,k)=1} h*h' - phi(k))/k and h*h' == 1
 *            (mod k), 1<=h,h'<k. All of the work is done in native integers.
 *            Pairs h and k-h are handled together, and the inverses come from
 *            one batch inversion for each k
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A074645_terms = 0;
static size_t A074645_cnt = 0, A074645_cap = 0;
static uint64_t A074645_next = 1;

static unsigned char *A074645_mark = 0;
static uint64_t *A074645_u = 0, *A074645_pref = 0;
static size_t A074645_bufcap = 0;

static uint64_t A074645_inv(uint64_t a, uint64_t m)
{
    int64_t r0 = (int64_t)a, r1 = (int64_t)m, s0 = 1, s1 = 0;
    while (r1 != 0) {
        int64_t q = r0 / r1, t;
        t = r0 - q * r1; r0 = r1; r1 = t;
        t = s0 - q * s1; s0 = s1; s1 = t;
    }
    s0 %= (int64_t)m;
    if (s0 < 0) s0 += (int64_t)m;
    return (uint64_t)s0;
}

static int A074645_test(uint64_t k)
{
    uint64_t half, phi, m, p, i, cnt, all;
    uint64_t primes[64];
    int np = 0, j;
    unsigned __int128 T = 0, S;
    if (k <= 2) return 1;
    half = (k - 1) / 2;
    if (half + 1 > A074645_bufcap) {
        size_t nc = (size_t)(half + 1) * 2;
        A074645_mark = (unsigned char *)realloc(A074645_mark, nc);
        A074645_u = (uint64_t *)realloc(A074645_u, nc * sizeof(uint64_t));
        A074645_pref = (uint64_t *)realloc(A074645_pref, nc * sizeof(uint64_t));
        A074645_bufcap = nc;
    }
    phi = k; m = k;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            primes[np++] = p;
            while (m % p == 0) m /= p;
            phi -= phi / p;
        }
    }
    if (m > 1) { primes[np++] = m; phi -= phi / m; }
    memset(A074645_mark, 0, (size_t)(half + 1));
    for (j = 0; j < np; j++)
        for (i = primes[j]; i <= half; i += primes[j]) A074645_mark[i] = 1;
    cnt = 0;
    for (i = 1; i <= half; i++)
        if (!A074645_mark[i]) A074645_u[cnt++] = i;
    if (cnt == 0) return 1;
    A074645_pref[0] = A074645_u[0] % k;
    for (i = 1; i < cnt; i++)
        A074645_pref[i] = (uint64_t)(((unsigned __int128)A074645_pref[i - 1] * A074645_u[i]) % k);
    all = A074645_inv(A074645_pref[cnt - 1], k);
    for (i = cnt; i-- > 0;) {
        uint64_t h = A074645_u[i], hi;
        if (i > 0) {
            hi = (uint64_t)(((unsigned __int128)all * A074645_pref[i - 1]) % k);
            all = (uint64_t)(((unsigned __int128)all * h) % k);
        } else {
            hi = all;
        }
        T += (unsigned __int128)h * hi + (unsigned __int128)(k - h) * (k - hi);
    }
    S = (T - phi) / k;
    return (S % phi) == 0;
}

seq_t a(idx_t n)
{
    while (A074645_cnt < (size_t)n) {
        uint64_t k = A074645_next++;
        if (A074645_test(k)) {
            if (A074645_cnt == A074645_cap) {
                size_t nc = A074645_cap ? A074645_cap * 2 : 64;
                A074645_terms = (uint64_t *)realloc(A074645_terms, nc * sizeof(uint64_t));
                A074645_cap = nc;
            }
            A074645_terms[A074645_cnt++] = k;
        }
    }
    return (seq_t)A074645_terms[n - 1];
}
