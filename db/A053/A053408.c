/*
 * A053408  Numbers k such that A003266(k) + 1 is prime.
 * https://oeis.org/A053408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k such that A003266(k)+1 is prime, A003266(k) =
 *            Product_{i=1..k} F(i); computed by search with a private bignum
 *            Miller-Rabin test
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define A053408_ML 100
#define A053408_KMAX 90
#define A053408_MAXT 64

typedef uint32_t a053408_u32;

static void a053408_mul(a053408_u32 *out, const a053408_u32 *a, int na, const a053408_u32 *b, int nb) {
    for (int i = 0; i < na + nb; i++) out[i] = 0;
    for (int i = 0; i < na; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < nb; j++) {
            uint64_t cur = (uint64_t)a[i] * b[j] + out[i + j] + carry;
            out[i + j] = (a053408_u32)cur;
            carry = cur >> 32;
        }
        out[i + nb] = (a053408_u32)carry;
    }
}

static int a053408_trim(const a053408_u32 *a, int n) {
    while (n > 1 && a[n - 1] == 0) n--;
    return n;
}

/* res = a*b mod N, all n limbs */
static void a053408_mulmod(a053408_u32 *res, const a053408_u32 *a, const a053408_u32 *b,
                           const a053408_u32 *N, int n) {
    a053408_u32 prod[2 * A053408_ML + 2];
    a053408_u32 r[A053408_ML + 2];
    a053408_mul(prod, a, n, b, n);
    for (int i = 0; i <= n; i++) r[i] = 0;
    for (int bit = 2 * n * 32 - 1; bit >= 0; bit--) {
        a053408_u32 inb = (prod[bit >> 5] >> (bit & 31)) & 1u;
        a053408_u32 c = inb;
        for (int i = 0; i <= n; i++) {
            a053408_u32 nc = r[i] >> 31;
            r[i] = (r[i] << 1) | c;
            c = nc;
        }
        /* compare r (n+1 limbs) with N (n limbs zero-extended) */
        bool ge;
        if (r[n] != 0) ge = true;
        else {
            ge = true;
            for (int i = n - 1; i >= 0; i--) {
                if (r[i] != N[i]) { ge = r[i] > N[i]; break; }
            }
        }
        if (ge) {
            int64_t borrow = 0;
            for (int i = 0; i <= n; i++) {
                int64_t cur = (int64_t)r[i] - (i < n ? (int64_t)N[i] : 0) - borrow;
                if (cur < 0) { cur += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
                r[i] = (a053408_u32)cur;
            }
        }
    }
    for (int i = 0; i < n; i++) res[i] = r[i];
}

static bool a053408_is_prime_big(const a053408_u32 *N, int n) {
    static const a053408_u32 sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
                                     101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199};
    if (n == 1 || (n == 2 && false)) {
        uint64_t v = N[0];
        if (v < 2) return false;
        for (uint64_t d = 2; d * d <= v; d++) if (v % d == 0) return false;
        return true;
    }
    for (size_t i = 0; i < sizeof(sp) / sizeof(sp[0]); i++) {
        uint64_t rem = 0;
        for (int j = n - 1; j >= 0; j--) rem = ((rem << 32) | N[j]) % sp[i];
        if (rem == 0) return false;
    }
    /* N-1 = d * 2^s */
    a053408_u32 d[A053408_ML + 2], nm1[A053408_ML + 2];
    for (int i = 0; i < n; i++) nm1[i] = N[i];
    for (int i = 0; i < n; i++) {
        if (nm1[i] != 0) { nm1[i]--; break; }
        nm1[i] = 0xFFFFFFFFu;
    }
    for (int i = 0; i < n; i++) d[i] = nm1[i];
    unsigned s = 0;
    while ((d[0] & 1u) == 0) {
        for (int i = 0; i < n; i++) {
            d[i] >>= 1;
            if (i + 1 < n) d[i] |= d[i + 1] << 31;
        }
        s++;
    }
    int topbit = n * 32 - 1;
    while (topbit >= 0 && !((d[topbit >> 5] >> (topbit & 31)) & 1u)) topbit--;
    static const a053408_u32 bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t bi = 0; bi < sizeof(bases) / sizeof(bases[0]); bi++) {
        a053408_u32 av[A053408_ML + 2], x[A053408_ML + 2], t[A053408_ML + 2];
        for (int i = 0; i < n; i++) { av[i] = 0; x[i] = 0; }
        av[0] = bases[bi];
        x[0] = 1;
        for (int b = topbit; b >= 0; b--) {
            a053408_mulmod(t, x, x, N, n);
            for (int i = 0; i < n; i++) x[i] = t[i];
            if ((d[b >> 5] >> (b & 31)) & 1u) {
                a053408_mulmod(t, x, av, N, n);
                for (int i = 0; i < n; i++) x[i] = t[i];
            }
        }
        bool one = (x[0] == 1), mone = (x[0] == nm1[0]);
        for (int i = 1; i < n; i++) {
            if (x[i] != 0) one = false;
            if (x[i] != nm1[i]) mone = false;
        }
        if (one || mone) continue;
        bool ok = false;
        for (unsigned r = 1; r < s; r++) {
            a053408_mulmod(t, x, x, N, n);
            for (int i = 0; i < n; i++) x[i] = t[i];
            bool m = true;
            for (int i = 0; i < n; i++) if (x[i] != nm1[i]) { m = false; break; }
            if (m) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static a053408_u32 P[A053408_ML + 4];
    static int plen = 1;
    static bool init = false;
    static uint64_t fprev = 0, fcurr = 1;
    static int k = 0;
    static unsigned long found[A053408_MAXT];
    static idx_t cnt = 0;

    if (!init) { init = true; P[0] = 1; plen = 1; }
    if (n < 1 || n > A053408_MAXT) return (seq_t)0;

    while (cnt < n && k < A053408_KMAX) {
        k++;
        /* multiply P by F(k) */
        a053408_u32 q[2];
        q[0] = (a053408_u32)(fcurr & 0xFFFFFFFFu);
        q[1] = (a053408_u32)(fcurr >> 32);
        a053408_u32 tmp[A053408_ML + 4];
        a053408_mul(tmp, P, plen, q, 2);
        int nl = a053408_trim(tmp, plen + 2);
        for (int i = 0; i < nl; i++) P[i] = tmp[i];
        plen = nl;
        uint64_t nf = fprev + fcurr;
        fprev = fcurr;
        fcurr = nf;

        /* N = P + 1 */
        a053408_u32 N[A053408_ML + 4];
        for (int i = 0; i < plen; i++) N[i] = P[i];
        N[plen] = 0;
        int nn = plen;
        uint64_t c = 1;
        for (int i = 0; i < plen && c; i++) {
            uint64_t cur = (uint64_t)N[i] + c;
            N[i] = (a053408_u32)cur;
            c = cur >> 32;
        }
        if (c) { N[plen] = 1; nn = plen + 1; }
        if (nn > A053408_ML) break;
        if (a053408_is_prime_big(N, nn)) found[cnt++] = (unsigned long)k;
    }
    if (n > cnt) return (seq_t)0;
    return (seq_t)found[n - 1];
}
