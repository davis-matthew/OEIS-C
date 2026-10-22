/*
 * A006686  Octavan primes: primes of the form p = x^8 + y^8.
 * https://oeis.org/A006686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime of the form x^8 + y^8; computed by enumerating
 *            x <= y <= M (M doubled as needed), keeping prime values <= M^8
 *            (trial division + Miller-Rabin), and sorting
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 1391,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_l, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/gcd_l.h"

typedef unsigned __int128 u128_t;

static u128_t *oct_cache = NULL;
static idx_t oct_len = 0;
static long oct_M = 0;

static int *oct_small = NULL;
static int oct_nsmall = 0;

static int bitlen128(u128_t v) {
    int b = 0;
    while (v) { b++; v >>= 1; }
    return b;
}

static u128_t mulmod128(u128_t a, u128_t b, u128_t m, int s) {
    if ((m >> 64) == 0) return (a * b) % m;
    u128_t mask = (((u128_t)1) << s) - 1;
    int nb = bitlen128(b);
    int c = (nb + s - 1) / s;
    u128_t r = 0;
    int i;
    for (i = c - 1; i >= 0; i--) {
        r = (r << s) % m;
        r = (r + a * ((b >> (i * s)) & mask)) % m;
    }
    return r;
}

static u128_t powmod128(u128_t b, u128_t e, u128_t m, int s) {
    u128_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod128(r, b, m, s);
        e >>= 1;
        if (e) b = mulmod128(b, b, m, s);
    }
    return r;
}

static bool mr_prime(u128_t n) {
    static const unsigned bases[20] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                                       31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
    int i, j, k, nbases, s;
    u128_t d;
    if (n < 2) return false;
    for (i = 0; i < 20; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    s = 127 - bitlen128(n);
    if (s < 1) s = 1;
    d = n - 1;
    k = 0;
    while ((d & 1) == 0) { d >>= 1; k++; }
    /* first 13 prime bases are deterministic below 3.3e24 */
    nbases = (n < (u128_t)3317044064679887385ULL * 1000000ULL) ? 13 : 20;
    for (i = 0; i < nbases; i++) {
        u128_t x = powmod128(bases[i], d, n, s);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < k; j++) {
            x = mulmod128(x, x, n, s);
            if (x == n - 1) break;
        }
        if (j >= k) return false;
    }
    return true;
}

static void init_small(void) {
    int lim = 20000, i, j;
    char *comp;
    if (oct_small) return;
    comp = (char *)calloc((size_t)lim + 1, 1);
    oct_small = (int *)malloc(sizeof(int) * (size_t)lim);
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        if (i % 16 == 1) oct_small[oct_nsmall++] = i;
        for (j = i * 2; j <= lim; j += i) comp[j] = 1;
    }
    free(comp);
}

static bool oct_isprime(u128_t v) {
    int i;
    if (v == 2) return true;
    if (v < 2 || (v & 1) == 0) return false;
    /* odd prime factors of x^8+y^8 with gcd(x,y)=1 are all 1 mod 16 */
    for (i = 0; i < oct_nsmall; i++) {
        u128_t p = (u128_t)oct_small[i];
        if (p * p > v) return true;
        if (v % p == 0) return v == p;
    }
    return mr_prime(v);
}

static int cmp_u128(const void *a, const void *b) {
    u128_t va = *(const u128_t *)a, vb = *(const u128_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static void build(long M) {
    u128_t *p8 = (u128_t *)malloc(sizeof(u128_t) * (size_t)(M + 1));
    u128_t *res;
    size_t cap = 1024, cnt = 0;
    u128_t L;
    long x, y;
    int i;
    for (x = 0; x <= M; x++) {
        u128_t v = 1;
        for (i = 0; i < 8; i++) v *= (u128_t)x;
        p8[x] = v;
    }
    L = p8[M];
    res = (u128_t *)malloc(sizeof(u128_t) * cap);
    for (y = 1; y <= M; y++) {
        for (x = 0; x <= y; x++) {
            u128_t v;
            if (!(x == 1 && y == 1)) {
                if (((x + y) & 1) == 0) continue;
                if (gcd_l(x, y) != 1) continue;
            }
            v = p8[x] + p8[y];
            if (v > L) break;
            if (!oct_isprime(v)) continue;
            if (cnt == cap) {
                cap *= 2;
                res = (u128_t *)realloc(res, sizeof(u128_t) * cap);
            }
            res[cnt++] = v;
        }
    }
    free(p8);
    qsort(res, cnt, sizeof(u128_t), cmp_u128);
    if (oct_cache) free(oct_cache);
    oct_cache = res;
    oct_len = (idx_t)cnt;
    oct_M = M;
}

seq_t a(idx_t n) {
    init_small();
    if (oct_M == 0) build(32);
    while (oct_len < n) build(oct_M * 2);
    return u128_to_seq(oct_cache[n - 1]);
}
