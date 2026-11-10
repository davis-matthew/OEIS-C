/*
 * A085316  Duplicate of A006686.
 * https://oeis.org/A085316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A006686(n); duplicate of A006686: n-th prime of the form
 *            x^8 + y^8, found by search over x <= y <= M and sorting
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   gcd_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_i64.h"

static seq_t A006686_u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

typedef unsigned __int128 A006686_u128_t;

static A006686_u128_t *A006686_oct_cache = NULL;
static idx_t A006686_oct_len = 0;
static long A006686_oct_M = 0;

static int *A006686_oct_small = NULL;
static int A006686_oct_nsmall = 0;

static int A006686_bitlen128(A006686_u128_t v) {
    int b = 0;
    while (v) { b++; v >>= 1; }
    return b;
}

static A006686_u128_t A006686_mulmod128(A006686_u128_t a, A006686_u128_t b, A006686_u128_t m, int s) {
    if ((m >> 64) == 0) return (a * b) % m;
    A006686_u128_t mask = (((A006686_u128_t)1) << s) - 1;
    int nb = A006686_bitlen128(b);
    int c = (nb + s - 1) / s;
    A006686_u128_t r = 0;
    int i;
    for (i = c - 1; i >= 0; i--) {
        r = (r << s) % m;
        r = (r + a * ((b >> (i * s)) & mask)) % m;
    }
    return r;
}

static A006686_u128_t A006686_powmod128(A006686_u128_t b, A006686_u128_t e, A006686_u128_t m, int s) {
    A006686_u128_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A006686_mulmod128(r, b, m, s);
        e >>= 1;
        if (e) b = A006686_mulmod128(b, b, m, s);
    }
    return r;
}

static bool A006686_mr_prime(A006686_u128_t n) {
    static const unsigned bases[20] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                                       31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
    int i, j, k, nbases, s;
    A006686_u128_t d;
    if (n < 2) return false;
    for (i = 0; i < 20; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    s = 127 - A006686_bitlen128(n);
    if (s < 1) s = 1;
    d = n - 1;
    k = 0;
    while ((d & 1) == 0) { d >>= 1; k++; }
    nbases = (n < (A006686_u128_t)3317044064679887385ULL * 1000000ULL) ? 13 : 20;
    for (i = 0; i < nbases; i++) {
        A006686_u128_t x = A006686_powmod128(bases[i], d, n, s);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < k; j++) {
            x = A006686_mulmod128(x, x, n, s);
            if (x == n - 1) break;
        }
        if (j >= k) return false;
    }
    return true;
}

static void A006686_init_small(void) {
    int lim = 20000, i, j;
    char *comp;
    if (A006686_oct_small) return;
    comp = (char *)calloc((size_t)lim + 1, 1);
    A006686_oct_small = (int *)malloc(sizeof(int) * (size_t)lim);
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        if (i % 16 == 1) A006686_oct_small[A006686_oct_nsmall++] = i;
        for (j = i * 2; j <= lim; j += i) comp[j] = 1;
    }
    free(comp);
}

static bool A006686_oct_isprime(A006686_u128_t v) {
    int i;
    if (v == 2) return true;
    if (v < 2 || (v & 1) == 0) return false;
    for (i = 0; i < A006686_oct_nsmall; i++) {
        A006686_u128_t p = (A006686_u128_t)A006686_oct_small[i];
        if (p * p > v) return true;
        if (v % p == 0) return v == p;
    }
    return A006686_mr_prime(v);
}

static int A006686_cmp_u128(const void *a, const void *b) {
    A006686_u128_t va = *(const A006686_u128_t *)a, vb = *(const A006686_u128_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static void A006686_build(long M) {
    A006686_u128_t *p8 = (A006686_u128_t *)malloc(sizeof(A006686_u128_t) * (size_t)(M + 1));
    A006686_u128_t *res;
    size_t cap = 1024, cnt = 0;
    A006686_u128_t L;
    long x, y;
    int i;
    for (x = 0; x <= M; x++) {
        A006686_u128_t v = 1;
        for (i = 0; i < 8; i++) v *= (A006686_u128_t)x;
        p8[x] = v;
    }
    L = p8[M];
    res = (A006686_u128_t *)malloc(sizeof(A006686_u128_t) * cap);
    for (y = 1; y <= M; y++) {
        for (x = 0; x <= y; x++) {
            A006686_u128_t v;
            if (!(x == 1 && y == 1)) {
                if (((x + y) & 1) == 0) continue;
                if (gcd_i64(x, y) != 1) continue;
            }
            v = p8[x] + p8[y];
            if (v > L) break;
            if (!A006686_oct_isprime(v)) continue;
            if (cnt == cap) {
                cap *= 2;
                res = (A006686_u128_t *)realloc(res, sizeof(A006686_u128_t) * cap);
            }
            res[cnt++] = v;
        }
    }
    free(p8);
    qsort(res, cnt, sizeof(A006686_u128_t), A006686_cmp_u128);
    if (A006686_oct_cache) free(A006686_oct_cache);
    A006686_oct_cache = res;
    A006686_oct_len = (idx_t)cnt;
    A006686_oct_M = M;
}

static seq_t A006686_a(idx_t n) {
    A006686_init_small();
    if (A006686_oct_M == 0) A006686_build(32);
    while (A006686_oct_len < n) A006686_build(A006686_oct_M * 2);
    return A006686_u128_to_seq(A006686_oct_cache[n - 1]);
}

seq_t a(idx_t n) {
    return A006686_a(n);
}
