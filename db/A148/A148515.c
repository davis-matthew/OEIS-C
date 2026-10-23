/*
 * A148515  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 0,
 *          -1), (0, -1, 1), (0, 1, 0), (1, -1, 0)}.
 * https://oeis.org/A148515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   Number of walks of length n in N^3 with steps * S = {(-1,-1,0),
 *            (-1,0,-1), (0,-1,1), (0,1,0), (1,-1,0)}. * We compute it by
 *            first generating the first 70 terms by brute force * and then
 *            finding a linear recurrence (minimal). The recurrence is * used
 *            for all larger n (n ≤ 10000). The implementatio...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   precompute_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/precompute_terms.h"

/* ---------- Helper functions for modular arithmetic ---------- */

static inline uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned long long)((__int128)a * b % mod);
}

static inline uint64_t mod_pow_u64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    while (e)
    {
        if (e & 1) r = mod_mul_u64(r, a, mod);
        a = mod_mul_u64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static inline uint64_t mod_inv_u64(uint64_t a, uint64_t mod)
{
    return mod_pow_u64(a, mod - 2, mod); /* mod is prime   */
}

/* ---------- Berlekamp-Massey for one prime modulus ---------- */

static void bm_mod(const uint64_t *s, size_t sz, uint64_t mod,
                   uint64_t *rec, size_t *rec_len)
{
    /* C and B hold the current and previous recurrences */
    uint64_t C[200], B[200];
    size_t L = 0, m = 1;
    uint64_t b = 1;
    memset(C, 0, sizeof(C));
    memset(B, 0, sizeof(B));
    C[0] = 1;   /* C[0] is always 1 */
    B[0] = 1;

    for (size_t n = 0; n < sz; ++n)
    {
        uint64_t d = s[n] % mod;
        for (size_t i = 1; i <= L; ++i)
            d = (d + mod_mul_u64(C[i], s[n - i], mod)) % mod;

        if (d == 0)
        {
            ++m;
            continue;
        }

        uint64_t coef = mod_mul_u64(d, mod_inv_u64(b, mod), mod);
        /* ensure C is large enough */
        if (m + B[0] > 199) break; /* should not happen for our small size */
        for (size_t i = 0; i < B[0]; ++i)
        {
            uint64_t idx = i + m;
            uint64_t val = C[idx] ? C[idx] : 0;
            val = (val + mod - mod_mul_u64(coef, B[i], mod)) % mod;
            C[idx] = val;
        }

        if (2 * L <= n)
        {
            memcpy(B, C, (L + 1) * sizeof(uint64_t));
            L = (int)(n + 1 - L);
            b = d;
            m = 1;
        }
        else
        {
            ++m;
        }
    }

    /* Recurrence coefficients are C[1..L] (C[0] is 1). */
    *rec_len = L;
    for (size_t i = 0; i < L; ++i)
        rec[i] = C[i + 1];
}

/* ---------- Garner reconstructed integer from moduli ---------- */

static long long crt_garner(const uint64_t *mods, const uint64_t *rem, size_t len)
{
    __int128 x = rem[0];
    __int128 M = mods[0];
    for (size_t i = 1; i < len; ++i)
    {
        uint64_t mi = mods[i];
        int64_t inv = mod_inv_u64((uint64_t)(M % mi), mi);
        uint64_t t = (uint64_t)((mi + mi - (uint64_t)(x % mi)) % mi);
        t = (uint64_t)(mod_mul_u64(t, inv, mi));
        x = x + t * M;
        M = M * mi;
    }

    /* convert from [0, M) to signed integer */
    __int128 half = M >> 1;
    if (x > half) x = x - M;
    return (long long)x;
}

/* ---------- Precomputation of first 70 terms by brute force ---------- */

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Static data: precomputed terms, recurrence coeffs, etc. */
    static int initialized = 0;
    static seq_t pre[70];
    static long long rec_coef[200];
    static size_t rec_len;
    const size_t PRE_NUM = 70; /* number of precomputed terms */

    if (!initialized)
    {
        /* 1. Precompute first 70 terms (sufficient for BM). */
        precompute_terms(pre, PRE_NUM);

        /* 2. Find linear recurrence via BM over three primes. */
        const uint64_t primes[3] = {1000000007ULL, 1000000009ULL, 998244353ULL};
        uint64_t rec_mod[3][200];
        size_t rec_mod_len[3];
        for (int p = 0; p < 3; ++p)
            bm_mod((uint64_t *)pre, PRE_NUM, primes[p], rec_mod[p], &rec_mod_len[p]);

        /* Assumes all lengths are equal (they should be). */
        rec_len = rec_mod_len[0];
        for (int p = 1; p < 3; ++p)
            if (rec_len < rec_mod_len[p]) rec_len = rec_mod_len[p];

        /* 3. Recover integer coefficients via Garner. */
        for (size_t i = 0; i < rec_len; ++i)
        {
            uint64_t rems[3];
            for (int p = 0; p < 3; ++p)
            {
                uint64_t r = rec_mod[p][i] % primes[p];
                /* coefficient c satisfies c ≡ -r (mod prime) */
                if (r == 0) rems[p] = 0;
                else rems[p] = primes[p] - r;
            }
            rec_coef[i] = crt_garner(primes, rems, 3);
        }
        /* 4. All precomputed. */
        initialized = 1;
    }

    if (n < PRE_NUM)
        return pre[(size_t)n];

    /* Use recurrence for larger n. */
    size_t N = (size_t)n;
    seq_t *arr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    for (size_t i = 0; i < PRE_NUM; ++i) arr[i] = pre[i];

    for (size_t i = PRE_NUM; i <= N; ++i)
    {
        seq_t sum = 0;
        for (size_t j = 0; j < rec_len; ++j)
        {
            long long c = rec_coef[j];
            uint64_t abs_c = c >= 0 ? (uint64_t)c : (uint64_t)(-c);
            seq_t prod = arr[i - 1 - j] * (seq_t)abs_c;
            if (c >= 0) sum += prod;
            else sum -= prod;
        }
        arr[i] = sum;
    }

    seq_t result = arr[N];
    free(arr);
    return result;
}
