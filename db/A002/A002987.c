/*
 * A002987  a(1) = 1; a(2) = 2; a(n) == a(k) (mod n-k) for all 1 < k < n.
 * https://oeis.org/A002987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; for n >= 3, a(n) is the least positive x
 *            with x == a(n-q) (mod q) for every maximal prime power q = p^e
 *            <= n-1. This is equivalent to x == a(k) (mod n-k) for all 1 <= k
 *            < n. The system is solved by the Chinese remainder theorem
 *            (Garner's mixed radix)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   A_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/A_inv.h"

static uint32_t **A_dig = NULL;   /* mixed-radix digits t_0..t_{c-1} */
static uint32_t **A_rad = NULL;   /* radices q_0..q_{c-1} (last unused) */
static uint32_t *A_cnt = NULL;
static idx_t A_done = 0, A_cap = 0;
static unsigned char *A_comp = NULL;
static idx_t A_scap = 0;

static void A_sieve(idx_t N)
{
    idx_t i, j;
    if (N < A_scap) return;
    free(A_comp);
    A_scap = 2 * N + 16;
    A_comp = (unsigned char *)calloc((size_t)A_scap, 1);
    for (i = 2; i * i < A_scap; i++)
        if (!A_comp[i])
            for (j = i * i; j < A_scap; j += i) A_comp[j] = 1;
}

/* a(m) mod q from its stored mixed-radix form */
static uint64_t A_res(idx_t m, uint64_t q)
{
    long long i, c = (long long)A_cnt[m];
    uint64_t r = A_dig[m][c - 1] % q;
    for (i = c - 2; i >= 0; i--)
        r = (r * (A_rad[m][i] % q) + A_dig[m][i]) % q;
    return r;
}

static void A_compute(idx_t n)
{
    idx_t p, k = 0, i;
    long long j;
    uint64_t *qs, *rs;
    uint32_t *t, *rad;
    int allzero = 1;

    A_sieve(n);
    qs = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    rs = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    for (p = 2; p + 1 <= n; p++) {
        uint64_t q;
        if (A_comp[p]) continue;
        q = p;
        while (q * p <= n - 1) q *= p;
        qs[k] = q;
        rs[k] = A_res(n - q, q);
        k++;
    }
    t = (uint32_t *)malloc((size_t)(k + 1) * sizeof(uint32_t));
    rad = (uint32_t *)malloc((size_t)(k + 1) * sizeof(uint32_t));
    for (i = 0; i < k; i++) {
        uint64_t q = qs[i], x = 0, P = 1, d;
        if (i > 0) {
            x = t[i - 1] % q;
            for (j = (long long)i - 2; j >= 0; j--)
                x = (x * (qs[j] % q) + t[j]) % q;
        }
        for (j = 0; j < (long long)i; j++) P = (P * (qs[j] % q)) % q;
        d = (rs[i] % q + q - x) % q;
        d = (d * (uint64_t)A_inv((long long)P, (long long)q)) % q;
        t[i] = (uint32_t)d;
        rad[i] = (uint32_t)q;
        if (d) allzero = 0;
    }
    if (allzero) {
        t[k] = 1; rad[k] = 1;
        A_cnt[n] = (uint32_t)(k + 1);
    } else {
        A_cnt[n] = (uint32_t)k;
    }
    A_dig[n] = t;
    A_rad[n] = rad;
    free(qs);
    free(rs);
}

static void A_store_small(idx_t m, uint32_t v)
{
    A_dig[m] = (uint32_t *)malloc(sizeof(uint32_t));
    A_rad[m] = (uint32_t *)malloc(sizeof(uint32_t));
    A_dig[m][0] = v;
    A_rad[m][0] = 1;
    A_cnt[m] = 1;
}

seq_t a(idx_t n)
{
    seq_t v;
    long long i, c;
    if (n >= A_cap) {
        idx_t nc = (n + 1) * 2, m;
        uint32_t **nd = (uint32_t **)malloc((size_t)nc * sizeof(uint32_t *));
        uint32_t **nr = (uint32_t **)malloc((size_t)nc * sizeof(uint32_t *));
        uint32_t *nn = (uint32_t *)malloc((size_t)nc * sizeof(uint32_t));
        for (m = 0; m < A_cap; m++) { nd[m] = A_dig[m]; nr[m] = A_rad[m]; nn[m] = A_cnt[m]; }
        free(A_dig); free(A_rad); free(A_cnt);
        A_dig = nd; A_rad = nr; A_cnt = nn;
        A_cap = nc;
    }
    if (A_done == 0) {
        A_store_small(1, 1);
        A_store_small(2, 2);
        A_done = 2;
    }
    while (A_done < n) {
        A_done++;
        A_compute(A_done);
    }
    c = (long long)A_cnt[n];
    v = (seq_t)(idx_t)A_dig[n][c - 1];
    for (i = c - 2; i >= 0; i--)
        v = v * (seq_t)(idx_t)A_rad[n][i] + (seq_t)(idx_t)A_dig[n][i];
    return v;
}
