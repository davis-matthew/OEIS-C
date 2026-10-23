/*
 * A125339  Number of base 29 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of circular n‑digit numbers in base 29 whose *
 *            adjacent digits differ by at most 3. The sequence equals the
 *            trace * of the n‑th power of the 29×29 adjacency matrix with
 *            entries * A_ij=1 if |i−j|≤3 else 0 (including the diagonal). A
 *            recurrence * of order 29 is derived by Berlekamp–Ma...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   mat_mul_exact, mat_mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mat_mul_exact.h"
#include "../utils/mat_mul_mod.h"
 
/* seq_t and idx_t are supplied by the build; do not define them. */
 
/* ---------- Helper functions for modular arithmetic ---------- */
 
/* modular exponentiation (a^e mod m) for 64‑bit values */
static long long mod_pow(long long a, long long e, long long m)
{
    unsigned __int128 res = 1;
    unsigned __int128 base = (unsigned long long)a % (unsigned long long)m;
    while (e > 0)
    {
        if (e & 1) res = (res * base) % (unsigned long long)m;
        base = (base * base) % (unsigned long long)m;
        e >>= 1;
    }
    return (long long)res;
}
 
/* modular inverse using Fermat (m is prime) */
static long long mod_inv(long long a, long long m)
{
    return mod_pow(a, m - 2, m);
}
 
/* ---------- Berlekamp–Massey over a prime field ---------- */
static void berlekamp_massey(const long long *s, size_t n, long long mod,
                             int *C, size_t *rec_len)
{
    long long B[64] = {0}, Cmat[64] = {0};
    size_t B_len = 1, C_len = 1;
    B[0] = Cmat[0] = 1;
    long long L = 0, b = 1, m = 1;
    for (size_t i = 0; i < n; ++i)
    {
        long long d = 0;
        for (size_t j = 0; j <= (size_t)L; ++j)
        {
            unsigned __int128 prod = (unsigned __int128)Cmat[j] *
                                    (unsigned long long)s[i - j];
            d = (d + (long long)(prod % (unsigned long long)mod)) % mod;
        }
        if (d == 0)
        {
            ++m;
            continue;
        }
        long long coef = d * mod_inv(b, mod) % mod;
        /* copy Cmat to temporary */
        long long T[64];
        for (size_t j = 0; j < C_len; ++j) T[j] = Cmat[j];
        /* ensure Cmat has enough space */
        while (C_len < B_len + m) Cmat[C_len++] = 0;
        for (size_t j = 0; j < B_len; ++j)
        {
            long long tmp = (Cmat[j + m] - coef * B[j]) % mod;
            if (tmp < 0) tmp += mod;
            Cmat[j + m] = tmp;
        }
        if (2 * L <= (long long)i)
        {
            L = i + 1 - L;
            for (size_t j = 0; j < B_len; ++j) B[j] = T[j];
            B_len = C_len;
            b = d;
            m = 1;
        }
        else
        {
            ++m;
        }
    }
    *rec_len = (size_t)L;
    for (size_t i = 0; i < *rec_len; ++i)
        C[i] = (int)Cmat[i + 1];
}

/* ---------- Global state for memoisation ---------- */
#define MAX_K 29
#define MAX_TERMS (2*MAX_K + 1)            /* 59 */
/* Cache up to 20000 terms which comfortably covers the required range. */
#define MAX_N 20000

static bool ready = false;
static seq_t cache[MAX_N+1];
static size_t cache_len;
static seq_t coeffs[MAX_K];
static size_t k;                          /* recurrence order */

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!ready)   /* initialise once */
    {
        /* Step 1: compute first 59 exact terms and their values mod prime */
        const size_t dim = 29;
        const uint64_t MOD = 1000000007ULL;    /* large prime */
        static seq_t exactA[29][29];
        static uint64_t Amod[29][29];
        static seq_t Pexact[29][29];
        static uint64_t Pmod[29][29];
        static seq_t init_exact[MAX_TERMS];
        static long long init_mod[MAX_TERMS];
 
        /* Build adjacency matrix (exact and modulo) */
        for (size_t i = 0; i < dim; ++i)
            for (size_t j = 0; j < dim; ++j)
            {
                if ( (i>j? i-j:j-i) <= 3 )
                {
                    exactA[i][j] = (seq_t)1;
                    Amod[i][j]   = 1;
                }
                else
                {
                    exactA[i][j] = (seq_t)0;
                    Amod[i][j]   = 0;
                }
            }
        /* Identity matrices */
        for (size_t i = 0; i < dim; ++i)
            for (size_t j = 0; j < dim; ++j)
            {
                if (i==j)
                {
                    Pexact[i][j] = (seq_t)1;
                    Pmod[i][j]   = 1;
                }
                else
                {
                    Pexact[i][j] = (seq_t)0;
                    Pmod[i][j]   = 0;
                }
            }
        /* Term 0 */
        init_exact[0] = (seq_t)1;
        init_mod[0]   = 1;
        /* Compute successive powers */
        static seq_t tmpExact[29][29];
        static uint64_t tmpMod[29][29];
        for (size_t step = 1; step < MAX_TERMS; ++step)
        {
            mat_mul_exact(dim, Pexact, exactA, tmpExact);
            mat_mul_mod  (dim, Pmod,   Amod,   tmpMod, MOD);
            /* copy back to P matrices */
            for (size_t i = 0; i < dim; ++i)
                for (size_t j = 0; j < dim; ++j)
                {
                    Pexact[i][j] = tmpExact[i][j];
                    Pmod[i][j]   = tmpMod[i][j];
                }
            /* trace (exact) */
            seq_t trace = (seq_t)0;
            for (size_t i = 0; i < dim; ++i) trace += Pexact[i][i];
            init_exact[step] = trace;
            /* trace (mod) */
            uint64_t tmod = 0;
            for (size_t i = 0; i < dim; ++i) tmod += Pmod[i][i];
            tmod %= MOD;
            init_mod[step] = (long long)tmod;
        }
 
        /* Step 2: derive recurrence via Berlekamp–Massey over prime MOD */
        int C_mod[MAX_K];
        size_t rec_len;
        berlekamp_massey(init_mod, MAX_TERMS, MOD, C_mod, &rec_len);
        k = rec_len;
        /* Convert coefficients to signed seq_t */
        for (size_t i = 0; i < k; ++i)
        {
            long long val = C_mod[i];                       /* 0 <= val < MOD */
            long long signed_val = (val <= MOD/2) ? val : val - (long long)MOD;
            /* recurrence: a[n] = -C_i * a[n-i-1] */
            coeffs[i] = (seq_t)(-signed_val);
        }
        /* Step 3: initialise cache with first k exact terms */
        for (size_t i = 0; i < k && i < MAX_N; ++i)
            cache[i] = init_exact[i];
        cache_len = k;
        ready = true;
    }
 
    /* Return cached value if already computed */
    if (n < cache_len) return cache[n];
 
    /* Extend cache up to n */
    for (size_t i = cache_len; i <= n; ++i)
    {
        seq_t term = (seq_t)0;
        for (size_t j = 0; j < k; ++j)
            term += cache[i - 1 - j] * coeffs[j];
        cache[i] = term;
    }
    cache_len = n + 1;
    return cache[n];
}
