/*
 * A149890  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 1,
 *          -1), (0, 0, 1), (0, 1, 1), (1, -1, 0)}.
 * https://oeis.org/A149890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of restricted walks in N^3 with the 5 given steps
 *            The function computes a(n) for any n>=0 by first generating the
 *            terms up to a moderate length, deriving the minimal linear
 *            recurrence with Berlekamp–Massey modulo two large primes,
 *            reconciling the coefficients by Chinese remainder th...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------  standard includes  ---------- */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ----------  modular arithmetic helpers  ---------- */
static long long mod_pow(long long a, long long e, long long mod)
{
    long long r = 1 % mod;
    a %= mod;
    while (e > 0) {
        if (e & 1) r = (long long)((__int128)r * a % mod);
        a = (long long)((__int128)a * a % mod);
        e >>= 1;
    }
    return r;
}
static long long mod_inv(long long a, long long mod)
{
    /* mod must be prime */
    return mod_pow(a, mod - 2, mod);
}

/* ----------  Berlekamp–Massey over a field (mod prime)  ---------- */
#define BM_MAX_N 256
static void berlekamp_massey(const long long *s, size_t n, long long mod,
                             long long *coeff, size_t *len)
{
    static long long C[BM_MAX_N];
    static long long B[BM_MAX_N];
    size_t L = 0, m = 1;
    long long b = 1;

    for (size_t i = 0; i < n; ++i) C[i] = 0;
    for (size_t i = 0; i < n; ++i) B[i] = 0;
    C[0] = 1;
    B[0] = 1;

    for (size_t i = 0; i < n; ++i) {
        long long d = 0;
        for (size_t j = 0; j <= L; ++j) {
            d = (d + C[j] * s[i - j]) % mod;
        }
        if (d == 0) {
            ++m;
            continue;
        }
        long long coef = d * mod_inv(b, mod) % mod;
        /* ensure C has enough space (but BM_MAX_N >> n) */
        for (size_t j = 0; j <= L; ++j) {
            long long val = (C[m + j] - coef * B[j]) % mod;
            if (val < 0) val += mod;
            C[m + j] = val;
        }
        if (2 * L <= i) {
            L = i + 1 - L;
            memcpy(B, C, (m + L + 1) * sizeof(long long));
            b = d;
            m = 1;
        } else {
            ++m;
        }
    }
    *len = L;
    for (size_t i = 0; i < L; ++i) {
        coeff[i] = C[i + 1] % mod;
        if (coeff[i] < 0) coeff[i] += mod;
    }
}

/* ----------  CRT for two moduli  ---------- */
static unsigned __int128 crt_two(const unsigned long long a1, const unsigned long long a2,
                                 const unsigned long long p1, const unsigned long long p2)
{
    unsigned long long invp1 = mod_inv(p1 % p2, p2);
    unsigned long long diff = (a2 + p2 - (a1 % p2)) % p2;
    unsigned __int128 k = (unsigned __int128)diff * invp1 % p2;
    return (unsigned __int128)a1 + k * p1;
}

/* ----------  global cache and recurrence storage  ---------- */
static seq_t *seq_cache = NULL;           /* dynamically enlarged array of computed terms   */
static size_t seq_cache_len = 0;          /* number of terms currently stored (>=1)         */
static size_t seq_cache_cap = 0;          /* allocated capacity of seq_cache                */
static long long *rec_coef = NULL;        /* recurrence coefficients (signed 64‑bit)        */
static size_t rec_k = 0;                  /* order of recurrence                          */
static bool initialized = false;

/* enlarge cache when needed */
static void seq_cache_push(seq_t val)
{
    if (seq_cache_len >= seq_cache_cap) {
        seq_cache_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        seq_cache = (seq_t*)realloc(seq_cache, seq_cache_cap * sizeof(seq_t));
    }
    seq_cache[seq_cache_len++] = val;
}

/* ----------  generate first N0 terms by dynamic programming  ---------- */
static void init_sequence(void)
{
    const int N0 = 60;                          /* how many terms to generate directly */
    const int M  = N0 + 1;                      /* dimension per axis */
    const size_t S = (size_t)M * M * M;         /* total number of lattice points */

    /* step vectors   (-1,-1,0), (-1,1,-1), (0,0,1), (0,1,1), (1,-1,0) */
    const int dx[5] = { -1, -1, 0, 0, 1 };
    const int dy[5] = { -1,  1, 0, 1,-1 };
    const int dz[5] = {  0, -1, 1, 1, 0 };

    seq_t *cur = (seq_t*)calloc(S, sizeof(seq_t));
    seq_t *nxt = (seq_t*)calloc(S, sizeof(seq_t));

    /* start from origin */
    cur[0] = (seq_t)1;
    seq_cache_push((seq_t)1);                 /* a(0) */

    for (int step = 1; step <= N0; ++step) {
        /* zero nxt (calloc already zero, but we reuse array) */
        memset(nxt, 0, S * sizeof(seq_t));

        for (size_t i = 0; i < S; ++i) {
            if (cur[i] == 0) continue;

            /* recover coordinates from linear index */
            size_t rem = i % (M * M);
            size_t y   = rem / M;
            size_t z   = rem % M;
            size_t x   = i / (M * M);

            seq_t val = cur[i];

            for (int k = 0; k < 5; ++k) {
                int nx = (int)x + dx[k];
                int ny = (int)y + dy[k];
                int nz = (int)z + dz[k];
                if (nx < 0 || ny < 0 || nz < 0) continue;
                size_t idx = (size_t)nx * M * M + (size_t)ny * M + (size_t)nz;
                nxt[idx] += val;
            }
        }

        /* compute a(step) as sum over all entries of nxt */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < S; ++i) {
            if (nxt[i] != 0) total += nxt[i];
        }
        seq_cache_push(total);

        /* swap cur and nxt for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }
    free(cur);
    free(nxt);

    /* ----------  derive linear recurrence  ---------- */
    const long long p1 = 1000000007LL;      /* two large primes */
    const long long p2 = 1000000009LL;

    const size_t N = seq_cache_len;          /* N0+1 */

    long long *s1 = (long long*)malloc(N * sizeof(long long));
    long long *s2 = (long long*)malloc(N * sizeof(long long));
    for (size_t i = 0; i < N; ++i) {
        long long val = (long long)(seq_cache[i] % (seq_t)p1);
        s1[i] = val;
        val = (long long)(seq_cache[i] % (seq_t)p2);
        s2[i] = val;
    }

    long long coeff1[BM_MAX_N], coeff2[BM_MAX_N];
    size_t len1, len2;
    berlekamp_massey(s1, N, p1, coeff1, &len1);
    berlekamp_massey(s2, N, p2, coeff2, &len2);

    free(s1);
    free(s2);

    /* lengths must match */
    if (len1 != len2) {
        /* Should never happen if sequence is well‑defined */
        rec_k = 0;
        return;
    }

    rec_k = len1;
    rec_coef = (long long*)malloc(rec_k * sizeof(long long));
    unsigned __int128 prod = (unsigned __int128)p1 * (unsigned __int128)p2;

    for (size_t i = 0; i < rec_k; ++i) {
        /* reconstruct C[i+1] from the two moduli (CRT) */
        unsigned __int128 res = crt_two((unsigned long long)coeff1[i],
                                        (unsigned long long)coeff2[i],
                                        (unsigned long long)p1, (unsigned long long)p2);
        long long val = (long long)res;
        if (res > prod / 2) val -= (long long)prod;   /* sign correction */
        /* recurrence is a[n] = -(C[1]*a[n-1] + … + C[k]*a[n-k]) */
        rec_coef[i] = -val;
    }

    initialized = true;
}

/* ----------  public function required by Aufgabe ---------- */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    if ((size_t)n < seq_cache_len) return seq_cache[n];

    /* extend the cache up to n using the recurrence */
    for (size_t i = seq_cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < rec_k; ++j) {
            seq_t term = seq_cache[i - j - 1];
            long long c = rec_coef[j];
            if (c >= 0) sum += term * (seq_t)c;
            else        sum -= term * (seq_t)(-c);
        }
        seq_cache_push(sum);
    }

    return seq_cache[n];
}
