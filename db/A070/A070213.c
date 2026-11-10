/*
 * A070213  Numbers k such that A056542(k) is prime.
 * https://oeis.org/A070213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search: a(n) = n-th k such that A056542(k) is prime, where
 *            A056542(k) = k*A056542(k-1) + 1, A056542(1) = 0. The values are
 *            kept as multiprecision integers in 32-bit limbs. Each one is
 *            tested by trial division and then by strong probable-prime tests
 *            (Montgomery arithmetic). For odd k >= 3, A056542...
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A070213_N = NULL;     /* A056542(k), little-endian limbs */
static size_t A070213_L = 0, A070213_cap = 0;

static void A070213_step(uint32_t k)
{
    uint64_t carry = 1; /* the "+1" */
    size_t i;
    for (i = 0; i < A070213_L; i++) {
        uint64_t t = (uint64_t)A070213_N[i] * k + carry;
        A070213_N[i] = (uint32_t)t;
        carry = t >> 32;
    }
    while (carry) {
        if (A070213_L == A070213_cap) {
            A070213_cap = A070213_cap ? A070213_cap * 2 : 8;
            A070213_N = (uint32_t *)realloc(A070213_N, A070213_cap * sizeof(uint32_t));
            if (A070213_N == NULL) abort();
        }
        A070213_N[A070213_L++] = (uint32_t)carry;
        carry >>= 32;
    }
}

static int A070213_cmp(const uint32_t *x, const uint32_t *y, size_t L)
{
    size_t i = L;
    while (i-- > 0) {
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

static void A070213_sub(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t L)
{
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t d = (int64_t)x[i] - (int64_t)y[i] - borrow;
        if (d < 0) { d += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        r[i] = (uint32_t)d;
    }
}

static void A070213_addmod(uint32_t *r, const uint32_t *x, const uint32_t *y,
                           const uint32_t *N, size_t L)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t s = (uint64_t)x[i] + y[i] + c;
        r[i] = (uint32_t)s;
        c = s >> 32;
    }
    if (c || A070213_cmp(r, N, L) >= 0) A070213_sub(r, r, N, L);
}

static void A070213_mm(uint32_t *r, const uint32_t *x, const uint32_t *y,
                       const uint32_t *N, size_t L, uint32_t ninv, uint32_t *t)
{
    size_t i, j;
    uint64_t S;
    uint32_t C, m;
    for (i = 0; i < L + 2; i++) t[i] = 0;
    for (i = 0; i < L; i++) {
        C = 0;
        for (j = 0; j < L; j++) {
            S = (uint64_t)t[j] + (uint64_t)x[j] * y[i] + C;
            t[j] = (uint32_t)S; C = (uint32_t)(S >> 32);
        }
        S = (uint64_t)t[L] + C;
        t[L] = (uint32_t)S; t[L + 1] = (uint32_t)(S >> 32);
        m = t[0] * ninv;
        S = (uint64_t)t[0] + (uint64_t)m * N[0];
        C = (uint32_t)(S >> 32);
        for (j = 1; j < L; j++) {
            S = (uint64_t)t[j] + (uint64_t)m * N[j] + C;
            t[j - 1] = (uint32_t)S; C = (uint32_t)(S >> 32);
        }
        S = (uint64_t)t[L] + C;
        t[L - 1] = (uint32_t)S; C = (uint32_t)(S >> 32);
        t[L] = t[L + 1] + C;
    }
    if (t[L] || A070213_cmp(t, N, L) >= 0) A070213_sub(t, t, N, L);
    for (i = 0; i < L; i++) r[i] = t[i];
}

static uint32_t A070213_sp[200];
static int A070213_spc = 0;

static void A070213_init_sp(void)
{
    uint32_t p, q;
    if (A070213_spc) return;
    for (p = 2; p < 1000; p++) {
        bool pr = true;
        for (q = 2; q * q <= p; q++) if (p % q == 0) { pr = false; break; }
        if (pr) A070213_sp[A070213_spc++] = p;
    }
}

static bool A070213_is_prime(const uint32_t *N, size_t L)
{
    int i;
    size_t j;
    static const uint32_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint32_t *one, *mone, *x, *tmp, *t;
    uint32_t inv, ninv, n0;
    size_t topbit, s, pos;
    bool result = true;

    if (L == 0) return false;
    if (L == 1 && N[0] < 2) return false;
    A070213_init_sp();
    for (i = 0; i < A070213_spc; i++) {
        uint32_t p = A070213_sp[i];
        uint64_t r = 0;
        j = L;
        while (j-- > 0) r = ((r << 32) | N[j]) % p;
        if (r == 0) return (L == 1 && N[0] == p);
    }
    if (L == 1 && N[0] < 994009u) return true;

    one = (uint32_t *)malloc(L * sizeof(uint32_t));
    mone = (uint32_t *)malloc(L * sizeof(uint32_t));
    x = (uint32_t *)malloc(L * sizeof(uint32_t));
    tmp = (uint32_t *)malloc(L * sizeof(uint32_t));
    t = (uint32_t *)malloc((L + 2) * sizeof(uint32_t));
    if (!one || !mone || !x || !tmp || !t) abort();

    n0 = N[0];
    inv = n0;
    for (i = 0; i < 5; i++) inv *= 2u - n0 * inv;
    ninv = 0u - inv;

    for (j = 0; j < L; j++) one[j] = 0;
    one[0] = 1;
    for (j = 0; j < 32 * L; j++) A070213_addmod(one, one, one, N, L);
    A070213_sub(mone, N, one, L);

    topbit = 32 * L - 1;
    while (!((N[topbit / 32] >> (topbit % 32)) & 1u)) topbit--;
    s = 1;
    while (!((N[s / 32] >> (s % 32)) & 1u)) s++;

    for (i = 0; i < 12 && result; i++) {
        uint32_t b = bases[i];
        size_t r;
        bool ok = false;
        for (j = 0; j < L; j++) x[j] = one[j];
        pos = topbit + 1;
        while (pos-- > s) {
            A070213_mm(x, x, x, N, L, ninv, t);
            if ((N[pos / 32] >> (pos % 32)) & 1u) {
                uint32_t c;
                for (j = 0; j < L; j++) tmp[j] = x[j];
                for (c = 1; c < b; c++) A070213_addmod(x, x, tmp, N, L);
            }
        }
        if (A070213_cmp(x, one, L) == 0 || A070213_cmp(x, mone, L) == 0) ok = true;
        for (r = 1; r < s && !ok; r++) {
            A070213_mm(x, x, x, N, L, ninv, t);
            if (A070213_cmp(x, mone, L) == 0) { ok = true; break; }
            if (A070213_cmp(x, one, L) == 0) break;
        }
        if (!ok) result = false;
    }

    free(one); free(mone); free(x); free(tmp); free(t);
    return result;
}

seq_t a(idx_t n)
{
    static idx_t found[64];
    static idx_t cnt = 0;
    static idx_t k = 1;   /* A070213_N currently holds A056542(k); A056542(1) = 0 */

    while (cnt < n && cnt < 64) {
        k++;
        A070213_step((uint32_t)k);
        if ((k % 2) == 0 && A070213_is_prime(A070213_N, A070213_L))
            found[cnt++] = k;
    }
    return (seq_t)found[n - 1];
}
