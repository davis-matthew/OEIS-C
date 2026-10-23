/*
 * A148879  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 0,
 *          1), (0, 1, 1), (1, -1, -1), (1, 0, -1)}.
 * https://oeis.org/A148879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Order 1 C/C++ implementation of OEIS A148879
   Sequence defined by walks in the first octant starting at (0,0,0)
   with step set {(-1,-1,1),(-1,0,1),(0,1,1),(1,-1,-1),(1,0,-1)}.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- helper macros and constants ---------- */
#define T_MAX 120          /* number of terms built for BM (must be >= 2*L) */
#define VIS_CAP 20000      /* temporary visited storage capacity   */
#define MOD1 1000000007ULL
#define MOD2 1000000009ULL
static const int DX[5] = { -1, -1,  0,  1,  1 };
static const int DY[5] = { -1,  0,  1, -1,  0 };
static const int DZ[5] = {  1,  1,  1, -1, -1 };

/* ---------- modular arithmetic ---------- */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = (r * a) % mod;
        a = (a * a) % mod;
        e >>= 1;
    }
    return r;
}
static uint64_t modinv64(uint64_t a, uint64_t mod)
{
    return modpow64(a, mod - 2, mod);
}

/* ---------- Berlekamp–Massey (Cyclic version) ---------- */
static void berlekamp_massey(const uint32_t *s, int n, uint64_t mod,
                             int *rec_len, int64_t *rec)
{
    uint64_t C[64] = {0}, B[64] = {0};
    C[0] = B[0] = 1;
    int L = 0, m = 1;
    uint64_t b = 1;

    for (int i = 0; i < n; ++i) {
        uint64_t d = 0;
        for (int j = 0; j <= L; ++j)
            d = (d + C[j] * (uint64_t)s[i - j]) % mod;

        if (d == 0) {
            ++m;
            continue;
        }
        uint64_t coef = d * modinv64(b, mod) % mod;
        uint64_t T[64];
        memcpy(T, C, sizeof(C));
        for (int j = 0; j + m < 64; ++j) {
            uint64_t val = (C[j + m] + mod - (coef * B[j]) % mod) % mod;
            C[j + m] = val;
        }
        if (2 * L <= i) {
            L = i + 1 - L;
            memcpy(B, T, sizeof(B));
            b = d;
            m = 1;
        } else
            ++m;
    }

    *rec_len = L;
    for (int i = 0; i < L; ++i) {
        uint64_t coeff = (mod - C[i + 1]) % mod;
        rec[i] = (int64_t)coeff;
    }
}

/* ---------- power for seq_t ---------- */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1ULL) res *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return res;
}

/* ---------- linear recurrence evaluation (Kitamasa) ---------- */
static void poly_mul(const seq_t *A, const seq_t *B, const int64_t *coeff,
                     int L, seq_t *R)
{
    /* convolution */
    seq_t tmp[128] = {0};
    for (int i = 0; i < L; ++i) if (A[i] != 0)
        for (int j = 0; j < L; ++j) if (B[j] != 0)
            tmp[i + j] += A[i] * B[j];

    /* reduction modulo characteristic polynomial */
    for (int k = 2 * L - 2; k >= L; --k) if (tmp[k] != 0) {
        seq_t val = tmp[k];
        for (int i = 1; i <= L; ++i) {
            if (coeff[i - 1] >= 0)
                tmp[k - i] += val * (seq_t)coeff[i - 1];
            else
                tmp[k - i] -= val * (seq_t)(-coeff[i - 1]);
        }
    }

    for (int i = 0; i < L; ++i) R[i] = tmp[i];
}

static seq_t linear_term(const seq_t *init, const int64_t *coeff, int L,
                         idx_t n)
{
    if (n < (idx_t)L) return init[n];

    /*fold**
     * curr polynomial      (starting with 1)
     * pol polynomial       (representing shift)
     */
    seq_t curr[128] = {0};
    seq_t pol[128]  = {0};
    curr[0] = 1;
    if (L > 1) pol[1] = 1;
    else pol[0] = coeff[0];   /* L == 1 case */

    idx_t e = n;
    while (e) {
        if (e & 1ULL) {
            seq_t tmp[128];
            poly_mul(curr, pol, coeff, L, tmp);
            memcpy(curr, tmp, sizeof(tmp));
        }
        seq_t tmp[128];
        poly_mul(pol, pol, coeff, L, tmp);
        memcpy(pol, tmp, sizeof(tmp));
        e >>= 1ULL;
    }

    seq_t ans = 0;
    for (int i = 0; i < L; ++i)
        ans += curr[i] * init[i];
    return ans;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    static bool init_done = false;
    static seq_t *init_vals = NULL;    /* a[0..L-1] */
    static int64_t *rec_coeff = NULL;  /* recurrence coefficients c[0..L-1] */
    static int L = 0;

    if (!init_done) {
        /* -------------------- 1. Build first T_MAX terms modulo MOD1 and MOD2 -------------------- */
        size_t dim = T_MAX + 1;
        size_t size = dim * dim * dim;
        uint32_t *dp1 = (uint32_t*)calloc(size, sizeof(uint32_t));
        uint32_t *dp2 = (uint32_t*)calloc(size, sizeof(uint32_t));
        uint32_t *cur1 = dp1, *cur2 = dp2;
        uint32_t *nxt1 = (uint32_t*)calloc(size, sizeof(uint32_t));
        uint32_t *nxt2 = (uint32_t*)calloc(size, sizeof(uint32_t));

        int vis_cur[VIS_CAP], vis_nxt[VIS_CAP];
        size_t sz_cur = 0, sz_nxt = 0;
        size_t origin = 0; /* x=y=z=0 -> idx 0 */

        cur1[origin] = 1;
        cur2[origin] = 1;
        vis_cur[sz_cur++] = origin;

        uint32_t mod1_terms[T_MAX + 1];
        uint32_t mod2_terms[T_MAX + 1];
        mod1_terms[0] = 1;
        mod2_terms[0] = 1;

        for (idx_t step = 0; step < T_MAX; ++step) {
            sz_nxt = 0;
            for (size_t vi = 0; vi < sz_cur; ++vi) {
                size_t idx = vis_cur[vi];
                uint32_t v1 = cur1[idx];
                uint32_t v2 = cur2[idx];
                if (v1 == 0 && v2 == 0) continue;

                /* decode */
                size_t tmp = idx;
                int z = tmp % dim; tmp /= dim;
                int y = tmp % dim;
                int x = tmp / dim;

                for (int m = 0; m < 5; ++m) {
                    int nx = x + DX[m];
                    int ny = y + DY[m];
                    int nz = z + DZ[m];
                    if (nx < 0 || ny < 0 || nz < 0) continue;
                    size_t nidx = (nx * dim + ny) * dim + nz;
                    if (nxt1[nidx] == 0 && nxt2[nidx] == 0 && sz_nxt < VIS_CAP) {
                        vis_nxt[sz_nxt++] = nidx;
                    }
                    nxt1[nidx] = (nxt1[nidx] + v1) % MOD1;
                    nxt2[nidx] = (nxt2[nidx] + v2) % MOD2;
                }
            }

            /* sum for term step+1 */
            uint64_t sum1 = 0, sum2 = 0;
            for (size_t vi = 0; vi < sz_nxt; ++vi) {
                size_t idx = vis_nxt[vi];
                sum1 += nxt1[idx];
                sum2 += nxt2[idx];
            }
            mod1_terms[step + 1] = (uint32_t)(sum1 % MOD1);
            mod2_terms[step + 1] = (uint32_t)(sum2 % MOD2);

            /* reset cur */
            for (size_t vi = 0; vi < sz_cur; ++vi) {
                size_t idx = vis_cur[vi];
                cur1[idx] = 0;
                cur2[idx] = 0;
            }

            /* swap cur and nxt */
            memcpy(cur1, nxt1, size * sizeof(uint32_t));
            memcpy(cur2, nxt2, size * sizeof(uint32_t));
            sz_cur = sz_nxt;
            memcpy(vis_cur, vis_nxt, sz_cur * sizeof(int));
            memset(nxt1, 0, size * sizeof(uint32_t));
            memset(nxt2, 0, size * sizeof(uint32_t));
        }
        free(nxt1);
        free(nxt2);
        free(dp1);
        free(dp2);

        /* -------------------- 2. BM to find recurrence modulo primes -------------------- */
        int64_t rec1[64], rec2[64];
        int len1, len2;
        berlekamp_massey(mod1_terms, T_MAX + 1, MOD1, &len1, rec1);
        berlekamp_massey(mod2_terms, T_MAX + 1, MOD2, &len2, rec2);
        if (len1 != len2) {
            /* should not happen for small T_MAX; if it does, abort */
            rec_coeff = NULL;
            init_done = false;
            return 0;
        }
        L = len1;

        /* -------------------- 3. Recover integer coefficients via CRT -------------------- */
        rec_coeff = (int64_t*)malloc(L * sizeof(int64_t));
        uint64_t prod = MOD1 * MOD2;
        for (int i = 0; i < L; ++i) {
            uint64_t a1 = (uint64_t)rec1[i];
            uint64_t a2 = (uint64_t)rec2[i];
            uint64_t t = ((a2 + MOD2 - a1 % MOD2) % MOD2) * modinv64(MOD1 % MOD2, MOD2) % MOD2;
            uint64_t val = (t * MOD1 + a1) % prod;
            /* convert to signed */
            if (val > prod / 2) val -= prod;
            rec_coeff[i] = (int64_t)val;
        }

        /* -------------------- 4. Compute exact initial terms up to L-1 -------------------- */
        size_t dim2 = L;
        size_t size2 = dim2 * dim2 * dim2;
        seq_t *dp_ex1 = (seq_t*)calloc(size2, sizeof(seq_t));
        seq_t *dp_ex2 = (seq_t*)calloc(size2, sizeof(seq_t));
        seq_t *cur_ex = dp_ex1, *nxt_ex = dp_ex2;

        int vis_cur_e[VIS_CAP], vis_nxt_e[VIS_CAP];
        size_t sz_cur_e = 0, sz_nxt_e = 0;
        size_t origin_ex = 0;
        cur_ex[origin_ex] = 1;
        vis_cur_e[sz_cur_e++] = origin_ex;

        init_vals = (seq_t*)malloc(L * sizeof(seq_t));
        init_vals[0] = 1;

        for (size_t step = 0; step < (size_t)(L - 1); ++step) {
            sz_nxt_e = 0;
            for (size_t vi = 0; vi < sz_cur_e; ++vi) {
                size_t idx = vis_cur_e[vi];
                seq_t val = cur_ex[idx];
                if (val == 0) continue;

                /* decode */
                size_t tmp = idx;
                int z = tmp % dim2; tmp /= dim2;
                int y = tmp % dim2;
                int x = tmp / dim2;

                for (int m = 0; m < 5; ++m) {
                    int nx = x + DX[m];
                    int ny = y + DY[m];
                    int nz = z + DZ[m];
                    if (nx < 0 || ny < 0 || nz < 0) continue;
                    size_t nidx = (nx * dim2 + ny) * dim2 + nz;
                    if (nxt_ex[nidx] == 0 && sz_nxt_e < VIS_CAP) {
                        vis_nxt_e[sz_nxt_e++] = nidx;
                    }
                    nxt_ex[nidx] += val;
                }
            }

            seq_t sum = 0;
            for (size_t vi = 0; vi < sz_nxt_e; ++vi)
                sum += nxt_ex[vis_nxt_e[vi]];
            init_vals[step + 1] = sum;

            for (size_t vi = 0; vi < sz_cur_e; ++vi) {
                size_t idx = vis_cur_e[vi];
                cur_ex[idx] = 0;
            }

            memcpy(cur_ex, nxt_ex, size2 * sizeof(seq_t));
            sz_cur_e = sz_nxt_e;
            memcpy(vis_cur_e, vis_nxt_e, sz_cur_e * sizeof(int));
            memset(nxt_ex, 0, size2 * sizeof(seq_t));
        }

        free( dp_ex1 );
        free( dp_ex2 );

        init_done = true;
    }

    if (n < (idx_t)L) return init_vals[n];
    return linear_term(init_vals, rec_coeff, L, n);
}
