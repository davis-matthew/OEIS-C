/*
 * A001528  NPN-equivalence classes of switching functions of exactly n
 *          variables.
 * https://oeis.org/A001528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = T(n) - T(n-1) for n >= 1, a(0) = T(0) = 1, where T(n) =
 *            A000370(n) is the number of NPN classes of switching functions
 *            of n variables: T(n) = (1/(2^(n+1) n!)) * Sum_{g in B_n x Z_2}
 *            fix(g), Burnside's lemma summed over the signed cycle types of
 *            the hyperoctahedral group B_n (fix = 2^(#inpu...
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   npn_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/npn_gcd.h"

static int npn_k[64], npn_s[64], npn_np;
static uint32_t *npn_S;
static size_t npn_len;
static int npn_n;
static unsigned __int128 npn_G0;

/* number of inputs fixed by g^j (a power of two, or 0) */
static unsigned __int128 npn_fix(uint64_t j)
{
    int e = 0, i;
    for (i = 0; i < npn_np; i++) {
        uint64_t k = (uint64_t)npn_k[i];
        uint64_t g = j ? npn_gcd(j, k) : k;
        if (npn_s[i] && ((j / g) & 1)) return 0;
        e += (int)g;
    }
    return (unsigned __int128)1 << e;
}

static void npn_addshift(unsigned __int128 N, uint64_t c)
{
    size_t idx = (size_t)(c / 32);
    unsigned off = (unsigned)(c % 32);
    uint64_t hi = 0, carry = 0;
    size_t i;
    for (i = 0; i < 5; i++) {
        uint64_t part = (i < 4) ? (uint64_t)(uint32_t)(N >> (32 * i)) : 0;
        uint64_t w = (part << off) + hi;
        uint64_t s;
        hi = w >> 32;
        s = (uint64_t)npn_S[idx + i] + (uint32_t)w + carry;
        npn_S[idx + i] = (uint32_t)s;
        carry = s >> 32;
    }
    for (i = idx + 5; carry && i < npn_len; i++) {
        uint64_t s = (uint64_t)npn_S[i] + carry;
        npn_S[i] = (uint32_t)s;
        carry = s >> 32;
    }
}

static void npn_leaf(unsigned __int128 z)
{
    uint64_t L = 1, j, m, c;
    unsigned __int128 acc = 0, N;
    int i, allEven;
    for (i = 0; i < npn_np; i++) {
        uint64_t q = npn_s[i] ? 2 * (uint64_t)npn_k[i] : (uint64_t)npn_k[i];
        L = L / npn_gcd(L, q) * q;
    }
    for (j = 0; j < L; j++) acc += npn_fix(j);
    c = (uint64_t)(acc / L);
    m = L;
    while (!(m & 1)) m >>= 1;
    allEven = (npn_fix(m) == 0);
    N = npn_G0 / z;
    npn_addshift(N, c);
    if (allEven) npn_addshift(N, c);
}

static void npn_rec(int t, int rem, unsigned __int128 z)
{
    int k, m;
    unsigned __int128 zz = z;
    if (rem == 0) { npn_leaf(z); return; }
    if (t >= 2 * npn_n) return;
    k = t / 2 + 1;
    if (k > rem) return;
    npn_rec(t + 1, rem, z);
    for (m = 1; m * k <= rem; m++) {
        npn_k[npn_np] = k;
        npn_s[npn_np] = t & 1;
        npn_np++;
        zz *= (unsigned __int128)(2 * k) * (unsigned)m;
        npn_rec(t + 1, rem - m * k, zz);
    }
    npn_np -= m - 1;
}

static uint32_t *npn_T(int n, size_t *len)
{
    int k;
    size_t i, words;
    unsigned b, sh;
    npn_n = n;
    npn_G0 = 1;
    for (k = 1; k <= n; k++) npn_G0 *= (unsigned __int128)(2 * k);
    npn_len = ((size_t)1 << n) / 32 + 8;
    npn_S = (uint32_t *)calloc(npn_len, sizeof(uint32_t));
    npn_np = 0;
    npn_rec(0, n, 1);
    for (k = 2; k <= n; k++) {
        uint64_t r = 0;
        for (i = npn_len; i-- > 0;) {
            uint64_t cur = (r << 32) | npn_S[i];
            npn_S[i] = (uint32_t)(cur / (uint64_t)k);
            r = cur % (uint64_t)k;
        }
    }
    sh = (unsigned)n + 1;
    words = sh / 32;
    b = sh % 32;
    for (i = 0; i < npn_len; i++) {
        uint32_t lo = (i + words < npn_len) ? npn_S[i + words] : 0;
        uint32_t hi = (i + words + 1 < npn_len) ? npn_S[i + words + 1] : 0;
        npn_S[i] = b ? (uint32_t)((lo >> b) | ((uint64_t)hi << (32 - b))) : lo;
    }
    *len = npn_len;
    return npn_S;
}

seq_t a(idx_t n)
{
    size_t la, lb = 0, i;
    uint32_t *A, *B = NULL;
    seq_t r = 0;
    long long top;
    if (n > 20) return 0;
    A = npn_T((int)n, &la);
    if (n >= 1) {
        uint64_t borrow = 0;
        B = npn_T((int)n - 1, &lb);
        for (i = 0; i < la; i++) {
            uint64_t bv = (i < lb) ? B[i] : 0;
            uint64_t av = A[i];
            uint64_t d = av - bv - borrow;
            borrow = (av < bv + borrow) ? 1 : 0;
            A[i] = (uint32_t)d;
        }
        free(B);
    }
    top = -1;
    for (i = la; i-- > 0;) {
        if (A[i]) {
            int bit;
            for (bit = 31; bit >= 0; bit--)
                if ((A[i] >> bit) & 1) { top = (long long)i * 32 + bit; break; }
            break;
        }
    }
    for (; top >= 0; top--) {
        r = r + r;
        if ((A[top / 32] >> (top % 32)) & 1) r = r + 1;
    }
    free(A);
    return r;
}
