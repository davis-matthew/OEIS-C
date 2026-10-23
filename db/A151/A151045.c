/*
 * A151045  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, -1), (-1,
 *          -1, 0), (0, 1, 0), (0, 1, 1), (1, 0, 1)}.
 * https://oeis.org/A151045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of walks of length n in the first octant of Z³,
 *            with steps {(-1,-1,-1), (-1,-1,0), (0,1,0), (0,1,1), (1,0,1)}
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MODULO 9223372036854775783ULL  /* prime > 2^63 */

/* modular exponentiation (used for modular inverse) */
static unsigned long long mod_pow(unsigned long long a, unsigned long long e)
{
    unsigned __int128 res = 1;
    unsigned __int128 base = a;
    while (e) {
        if (e & 1)
            res = (res * base) % MODULO;
        base = (base * base) % MODULO;
        e >>= 1;
    }
    return (unsigned long long)res;
}
static unsigned long long mod_inv(unsigned long long a)
{
    return mod_pow(a, MODULO - 2);
}

/* Berlekamp–Massey over the field modulo MODULO.
   Input: sequence s[0..N-1] of unsigned long long (mod MODULO).
   Output: recurrence coefficients rec[0..L-1] ,   a[n] = Σ rec[i]*a[n-1-i]  (mod MODULO)   */
static void berlekamp_massey(const unsigned long long *s, int N,
                             long long *rec, int *rec_len)
{
    unsigned long long *C = (unsigned long long*)calloc(N, sizeof(unsigned long long));
    unsigned long long *B = (unsigned long long*)calloc(N, sizeof(unsigned long long));
    unsigned long long *oldC;
    C[0] = 1;
    B[0] = 1;
    int L = 0;
    int m = 1;
    unsigned long long b = 1;

    for (int n = 0; n < N; ++n) {
        unsigned long long d = s[n];
        for (int i = 1; i <= L; ++i) {
            unsigned long long mul = (unsigned __int128)C[i] * s[n - i] % MODULO;
            d += mul;
            if (d >= MODULO) d -= MODULO;
        }
        if (d == 0) {
            ++m;
            continue;
        }
        unsigned long long coef = (unsigned __int128)d * mod_inv(b) % MODULO;
        /* copy C to oldC (will be new B if improvement) */
        oldC = (unsigned long long*)malloc(N * sizeof(unsigned long long));
        memcpy(oldC, C, N * sizeof(unsigned long long));

        /* update C :  C = C - coef * x^m * B  (mod MODULO)  */
        for (int i = 0; i + m < N; ++i) {
            unsigned long long mul = (unsigned __int128)coef * B[i] % MODULO;
            unsigned long long val = C[i + m] + MODULO - mul;
            if (val >= MODULO) val -= MODULO;
            C[i + m] = val;
        }

        if (2 * L <= n) {
            L = n + 1 - L;
            memcpy(B, oldC, N * sizeof(unsigned long long));
            b = d;
            m = 1;
        } else {
            ++m;
        }
        free(oldC);
    }

    /* extract recurrence coefficients */
    *rec_len = L;
    for (int i = 0; i < L; ++i) {
        unsigned long long val = (MODULO - C[i + 1]) % MODULO;   /* -C[i+1] mod MODULO */
        long long signed_val = (long long)val;
        if (signed_val > (long long)(MODULO / 2))
            signed_val -= (long long)MODULO;   /* bring to signed range */
        rec[i] = signed_val;
    }
    free(C);
    free(B);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static storage for computed sequence and recurrence */
    static int initialized = 0;
    static seq_t *seq_cache = NULL;      /* sequence values */
    static size_t cache_len = 0, cache_cap = 0;
    static long long rec_coeffs[200];
    static int rec_len = 0;

    if (!initialized) {
        /* ---- 1. Compute first terms by explicit DP -------------------------------- */
        const int INIT_N = 80;        /* number of terms to compute directly */
        const int DIM = INIT_N + 1;
        const size_t TOTAL = (size_t)DIM * DIM * DIM;

        /* allocate 3‑D arrays as linear vectors */
        seq_t *cur = (seq_t*)malloc(TOTAL * sizeof(seq_t));
        seq_t *nxt = (seq_t*)malloc(TOTAL * sizeof(seq_t));

        /* initialise cur with zeros */
        for (size_t i = 0; i < TOTAL; ++i) cur[i] = (seq_t)0;
        cur[0] = (seq_t)1;          /* start at (0,0,0)  */

        /* cache will hold INIT_N+1 terms */
        cache_cap = INIT_N + 1;
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)1;     /* a(0) = 1 */
        cache_len  = 1;

        for (int step = 1; step <= INIT_N; ++step) {
            for (size_t i = 0; i < TOTAL; ++i) nxt[i] = (seq_t)0;
            seq_t sum = (seq_t)0;

            /* loop over all coordinates in the cube */
            for (int x = 0; x <= INIT_N; ++x) {
                size_t base_x = (size_t)x * DIM * DIM;
                for (int y = 0; y <= INIT_N; ++y) {
                    size_t base_xy = base_x + (size_t)y * DIM;
                    for (int z = 0; z <= INIT_N; ++z) {
                        seq_t v = cur[base_xy + z];
                        if (!v) continue;
                        /* S1 : (-1,-1,-1) */
                        if (x > 0 && y > 0 && z > 0) {
                            size_t idx = (size_t)(x-1) * DIM * DIM
                                       + (size_t)(y-1) * DIM
                                       + (size_t)(z-1);
                            nxt[idx] += v;
                            sum += v;
                        }
                        /* S2 : (-1,-1,0) */
                        if (x > 0 && y > 0) {
                            size_t idx = (size_t)(x-1) * DIM * DIM
                                       + (size_t)(y-1) * DIM
                                       + (size_t)z;
                            nxt[idx] += v;
                            sum += v;
                        }
                        /* S3 : (0,1,0) */
                        if (y + 1 <= INIT_N) {
                            size_t idx = (size_t)x * DIM * DIM
                                       + (size_t)(y+1) * DIM
                                       + (size_t)z;
                            nxt[idx] += v;
                            sum += v;
                        }
                        /* S4 : (0,1,1) */
                        if (y + 1 <= INIT_N && z + 1 <= INIT_N) {
                            size_t idx = (size_t)x * DIM * DIM
                                       + (size_t)(y+1) * DIM
                                       + (size_t)(z+1);
                            nxt[idx] += v;
                            sum += v;
                        }
                        /* S5 : (1,0,1) */
                        if (x + 1 <= INIT_N && z + 1 <= INIT_N) {
                            size_t idx = (size_t)(x+1) * DIM * DIM
                                       + (size_t)y * DIM
                                       + (size_t)(z+1);
                            nxt[idx] += v;
                            sum += v;
                        }
                    }
                }
            }
            seq_cache[step] = sum;
            ++cache_len;
            /* swap cur and nxt for next iteration */
            seq_t *tmp = cur; cur = nxt; nxt = tmp;
        }

        free(cur);
        free(nxt);

        /* ---- 2. Find linear recurrence from initial terms -------------------------- */
        unsigned long long *seq_mod = (unsigned long long*)malloc(cache_len * sizeof(unsigned long long));
        for (size_t i = 0; i < cache_len; ++i)
            seq_mod[i] = (unsigned long long)(seq_cache[i] % MODULO);

        berlekamp_massey(seq_mod, (int)cache_len, rec_coeffs, &rec_len);
        free(seq_mod);

        initialized = 1;
    }

    /* If the requested n is already in the cache, return immediately */
    if (n < (idx_t)cache_len)
        return seq_cache[n];

    /* ---- 3. Extend the cache using the discovered recurrence -------------------- */
    /* ensure capacity */
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            seq_cache = (seq_t*)realloc(seq_cache, cache_cap * sizeof(seq_t));
        }
        size_t idx = cache_len;
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int j = 0; j < rec_len; ++j) {
            seq_t prod = seq_cache[idx - 1 - j] *
                         (rec_coeffs[j] >= 0 ?
                          (seq_t)rec_coeffs[j] :
                          (seq_t)(-rec_coeffs[j]));
            if (rec_coeffs[j] >= 0)
                pos += prod;
            else
                neg += prod;
        }
        seq_cache[idx] = pos - neg;   /* pos ≥ neg by construction      */
        ++cache_len;
    }
    return seq_cache[n];
}
