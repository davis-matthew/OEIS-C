/*
 * A188271  Number of nondecreasing strings of numbers x(i=1..n) in -3..3 with
 *          sum x(i)^3 equal to 0.
 * https://oeis.org/A188271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 when n < 1. For n ≥ 1 the sequence is defined by
 *            a(1)=1, a(2)=4, a(3)=4, a(4)=10, a(5)=10, …, a(24)=1899, and for
 *            n≥65 it satisfies a linear recurrence of order L (obtained by
 *            Berlekamp–Massey from the first 140 terms). The first 200 terms
 *            are computed exactly by a dynamic programming ov...
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   ext_gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ext_gcd_uint64.h"

static const int WEIGHTS[7] = { -27, -8, -1, 0, 1, 8, 27 };

/* Modular inverse modulo prime */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t x, y;
    ext_gcd_uint64(a, mod, &x, &y);
    x %= (int64_t)mod;
    if (x < 0) x += (int64_t)mod;
    return (uint64_t)x;
}

/* ============================================================= */
/*               Berlekamp–Massey over a prime modulus           */
/* ============================================================= */
static size_t bm_find_order(const int *seq, size_t n, int mod, int *rec)
{
    /* C = current, B = previous */
    int *C = (int*)malloc((n + 1) * sizeof(int));
    int *B = (int*)malloc((n + 1) * sizeof(int));
    int *T = (int*)malloc((n + 1) * sizeof(int));
    size_t L = 0;
    int m = 1;
    int b = 1;

    C[0] = 1;
    B[0] = 1;

    for (size_t i = 0; i < n; ++i)
    {
        int d = 0;
        for (size_t j = 0; j <= L; ++j)
        {
            d += (int)((int64_t)C[j] * seq[i - j] % mod);
            if (d >= mod) d -= mod;
        }
        if (d == 0)
        {
            ++m;
            continue;
        }
        int coef = (int)((int64_t)d * modinv_uint64(b, (uint64_t)mod) % mod);
        /* Copy C to T */
        for (size_t j = 0; j <= L; ++j) T[j] = C[j];
        /* Ensure C is large enough */
        if (L + m >= n) { free(C); free(B); free(T); return 0; }
        for (size_t j = 0; j <= L; ++j)
        {
            int val = (int)((int64_t)B[j] * coef % mod);
            int idx = j + m;
            C[idx] = (C[idx] - val) % mod;
            if (C[idx] < 0) C[idx] += mod;
        }
        if (2 * L <= i)
        {
            L = i + 1 - L;
            for (size_t j = 0; j <= L; ++j) B[j] = T[j];
            b = d;
            m = 1;
        }
        else
        {
            ++m;
        }
    }
    /* Store recurrence coefficients (reversed) */
    for (size_t i = 0; i < L; ++i)
    {
        int v = C[i + 1];
        if (v < 0) v += mod;
        int coef_mod = (mod - v) % mod;   /* because C[0] == 1 */
        /* Recover small signed coefficient */
        if (coef_mod > mod / 2) rec[i] = coef_mod - (int)mod;
        else rec[i] = coef_mod;
    }
    free(C);
    free(B);
    free(T);
    return L;
}

/* ============================================================= */
/*          Compute first N terms exactly by DP (N <= 200)        */
/* ============================================================= */
static void compute_first_terms(size_t N, seq_t *terms)
{
    const int OFFSET = 27 * N;
    const int SIZE = 2 * OFFSET + 1;
    const size_t TOTAL = (N + 1) * SIZE;

    seq_t *dp = (seq_t*)calloc(TOTAL, sizeof(seq_t));
    size_t offset = OFFSET;
    size_t stride = SIZE;

    dp[offset] = 1;   /* dp[0][OFFSET] = 1 */

    for (int widx = 0; widx < 7; ++widx)
    {
        int w = WEIGHTS[widx];
        for (size_t len = 0; len < N; ++len)
        {
            size_t base = len * stride;
            size_t next = (len + 1) * stride;
            for (int sum = 0; sum < SIZE; ++sum)
            {
                seq_t val = dp[base + sum];
                if (!val) continue;
                int ns = sum + w;
                if (ns < 0 || ns >= SIZE) continue;
                dp[next + ns] += val;
            }
        }
    }

    for (size_t i = 1; i <= N; ++i)
        terms[i] = dp[i * stride + offset];

    free(dp);
}

/* ============================================================= */
/*                       Main function a(n)                       */
/* ============================================================= */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t order = 0;
    static int *rec = NULL;

    /* Precomputation on first call */
    if (!initialized)
    {
        const size_t PRE_N = 200;          /* enough for BM and recurrence */
        seq_t *pre_terms = (seq_t*)malloc((PRE_N + 1) * sizeof(seq_t));

        compute_first_terms(PRE_N, pre_terms);

        /* Use first 140 terms for BM to find recurrence */
        const size_t BM_N = 140;
        int *BM_seq = (int*)malloc(BM_N * sizeof(int));
        for (size_t i = 0; i < BM_N; ++i)
            BM_seq[i] = (int)(pre_terms[i + 1] % 1000000007U);

        rec = (int*)malloc(BM_N * sizeof(int));
        order = bm_find_order(BM_seq, BM_N, 1000000007U, rec);

        /* Allocate cache and store first PRE_N terms */
        cache_size = PRE_N;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        for (size_t i = 1; i <= cache_size; ++i) cache[i] = pre_terms[i];

        free(pre_terms);
        free(BM_seq);
        initialized = true;
    }

    /* Return cached value if possible */
    if ((idx_t)n <= (idx_t)cache_size)
        return cache[(size_t)n];

    /* Extend cache up to n */
    size_t new_size = (size_t)n;
    cache = (seq_t*)realloc(cache, (new_size + 1) * sizeof(seq_t));

    for (size_t i = cache_size + 1; i <= new_size; ++i)
    {
        seq_t sum = 0;
        for (size_t j = 0; j < order; ++j)
        {
            int coeff = rec[j];
            if (coeff >= 0)
                sum += (seq_t)coeff * cache[i - j - 1];
            else
                sum -= (seq_t)(-coeff) * cache[i - j - 1];
        }
        cache[i] = sum;
    }
    cache_size = new_size;
    return cache[cache_size];
}
