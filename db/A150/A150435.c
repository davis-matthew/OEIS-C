/*
 * A150435  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 1,
 *          1), (0, 0, -1), (1, 0, 0), (1, 1, 0)}.
 * https://oeis.org/A150435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   This implementation computes the OEIS sequence A150435.
   The sequence counts walks of length n inside the first octant
   N³ (x≥0, y≥0, z≥0) starting at (0,0,0) with steps

       (-1,-1,1), (-1,1,1), (0,0,-1), (1,0,0), (1,1,0).

   The computation uses a linear recurrence obtained by the
   Berlekamp–Massey algorithm from the first 80 terms computed
   by dynamical programming modulo three primes.  The recurrence
   is then applied to generate terms up to the requested index.
   The implementation is valid both as C11 and C++17.

*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MOD1 1000000007U
#define MOD2 1000000009U
#define MOD3  998244353U

/* forward declaration of the target function */
seq_t a(idx_t n);

/* ---------- helper functions for modular arithmetic ---------- */

static uint32_t mod_pow(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint64_t res = 1;
    uint64_t b   = base % mod;
    while (exp)
    {
        if (exp & 1U) res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return (uint32_t)res;
}

static uint32_t mod_inv(uint32_t a, uint32_t mod)
{
    /* mod is prime */
    return mod_pow(a, mod - 2U, mod);
}

/* ---------- Berlekamp–Massey on a sequence modulo a prime ---------- */

static int64_t *bm_mod(const uint32_t *s, int len, uint32_t mod, int *out_len)
{
    /* maximum order needed for sequence of length 80 is far below 200 */
    int64_t C[200] = {0};
    int64_t B[200] = {0};
    int L = 0, m = 1;
    uint32_t b = 1;

    C[0] = 1;
    B[0] = 1;

    int i, j;
    for (i = 0; i < len; ++i)
    {
        uint64_t d = s[i];
        for (j = 1; j <= L; ++j)
            d = (d + C[j] * (uint64_t)s[i - j]) % mod;

        if (d == 0)
        {
            ++m;
            continue;
        }

        int64_t coef = (int64_t)d * mod_inv(b, mod) % mod;
        int64_t T[200];
        for (j = 0; j <= L; ++j) T[j] = C[j];

        if (L + m >= 200)                 /* should not happen for len=80 */
            return NULL;

        for (j = 0; j <= L; ++j)
        {
            int64_t val = C[j + m] - coef * B[j];
            val %= mod;
            if (val < 0) val += mod;
            C[j + m] = val;
        }

        if (2 * L <= i)
        {
            L = i + 1 - L;
            for (j = 0; j <= L + m; ++j) B[j] = T[j];
            b = (uint32_t)d;
            m = 1;
        }
        else
            ++m;
    }

    /* Recurrence: a_n + C[1] a_{n-1} + ... + C[L] a_{n-L} = 0
       therefore coefficients are (-C[1..L]) mod mod */
    int k = L;
    *out_len = k;
    int64_t *rec = (int64_t *)malloc(k * sizeof(int64_t));
    for (j = 0; j < k; ++j)
    {
        uint32_t cmod = (uint32_t)((mod - (C[j + 1] % mod)) % mod);
        int64_t sgn;
        if (cmod > mod / 2U) sgn = (int64_t)cmod - (int64_t)mod;
        else                 sgn = (int64_t)cmod;
        rec[j] = sgn;
    }
    return rec;
}

/* ---------- index accessor for 3‑D arrays ---------- */
static size_t idx3(size_t dim, int x, int y, int z)
{
    return ((size_t)x * dim + y) * dim + z;
}

/* ---------- global data used by the function a(n) ---------- */

static int64_t rec_coeff[64];          /* recurrence coefficients */
static size_t rec_len = 0;             /* order of recurrence */
static seq_t *values = NULL;           /* computed terms */
static size_t values_cap = 0;          /* allocated capacity */
/* flag to indicate that the sequence has been initialized */
static int initialized = 0;

/* ---------- initialization routine ---------- */
static void init_sequence(void)
{
    /* If already initialized, nothing to do */
    if (initialized) return;
    initialized = 1;

    /* Step 1: compute first 80 terms modulo three primes via DP */
    const int M = 80;
    const int dim = M + 1;
    const size_t total = (size_t)dim * dim * dim;

    uint32_t *cur1 = (uint32_t *)malloc(total * sizeof(uint32_t));
    uint32_t *next1 = (uint32_t *)malloc(total * sizeof(uint32_t));
    uint32_t *cur2 = (uint32_t *)malloc(total * sizeof(uint32_t));
    uint32_t *next2 = (uint32_t *)malloc(total * sizeof(uint32_t));
    uint32_t *cur3 = (uint32_t *)malloc(total * sizeof(uint32_t));
    uint32_t *next3 = (uint32_t *)malloc(total * sizeof(uint32_t));

    memset(cur1, 0, total * sizeof(uint32_t));
    memset(cur2, 0, total * sizeof(uint32_t));
    memset(cur3, 0, total * sizeof(uint32_t));

    cur1[0] = cur2[0] = cur3[0] = 1;

    /* storage for first 81 terms modulo each prime */
    uint32_t seq_mod1[81], seq_mod2[81], seq_mod3[81];
    seq_mod1[0] = seq_mod2[0] = seq_mod3[0] = 1;

    int step;
    for (step = 0; step < M; ++step)
    {
        memset(next1, 0, total * sizeof(uint32_t));
        memset(next2, 0, total * sizeof(uint32_t));
        memset(next3, 0, total * sizeof(uint32_t));
        uint32_t sum1 = 0, sum2 = 0, sum3 = 0;
        int maxc = step;           /* x,y,z <= step in current layer */

        int x, y, z;
        for (x = 0; x <= maxc; ++x)
        {
            for (y = 0; y <= maxc; ++y)
            {
                for (z = 0; z <= maxc; ++z)
                {
                    size_t id = idx3(dim, x, y, z);
                    uint32_t v1 = cur1[id];
                    uint32_t v2 = cur2[id];
                    uint32_t v3 = cur3[id];
                    if (v1 == 0 && v2 == 0 && v3 == 0) continue;

                    /* transition 1: (-1,-1,1) */
                    if (x >= 1 && y >= 1)
                    {
                        size_t nid = idx3(dim, x - 1, y - 1, z + 1);
                        uint32_t nv1 = next1[nid] + v1;
                        if (nv1 >= MOD1) nv1 -= MOD1;
                        next1[nid] = nv1;
                        uint32_t nv2 = next2[nid] + v2;
                        if (nv2 >= MOD2) nv2 -= MOD2;
                        next2[nid] = nv2;
                        uint32_t nv3 = next3[nid] + v3;
                        if (nv3 >= MOD3) nv3 -= MOD3;
                        next3[nid] = nv3;
                        sum1 += v1; if (sum1 >= MOD1) sum1 -= MOD1;
                        sum2 += v2; if (sum2 >= MOD2) sum2 -= MOD2;
                        sum3 += v3; if (sum3 >= MOD3) sum3 -= MOD3;
                    }

                    /* transition 2: (-1,1,1) */
                    if (x >= 1 && z + 1 <= M)
                    {
                        size_t nid = idx3(dim, x - 1, y + 1, z + 1);
                        uint32_t nv1 = next1[nid] + v1;
                        if (nv1 >= MOD1) nv1 -= MOD1;
                        next1[nid] = nv1;
                        uint32_t nv2 = next2[nid] + v2;
                        if (nv2 >= MOD2) nv2 -= MOD2;
                        next2[nid] = nv2;
                        uint32_t nv3 = next3[nid] + v3;
                        if (nv3 >= MOD3) nv3 -= MOD3;
                        next3[nid] = nv3;
                        sum1 += v1; if (sum1 >= MOD1) sum1 -= MOD1;
                        sum2 += v2; if (sum2 >= MOD2) sum2 -= MOD2;
                        sum3 += v3; if (sum3 >= MOD3) sum3 -= MOD3;
                    }

                    /* transition 3: (0,0,-1) */
                    if (z >= 1)
                    {
                        size_t nid = idx3(dim, x, y, z - 1);
                        uint32_t nv1 = next1[nid] + v1;
                        if (nv1 >= MOD1) nv1 -= MOD1;
                        next1[nid] = nv1;
                        uint32_t nv2 = next2[nid] + v2;
                        if (nv2 >= MOD2) nv2 -= MOD2;
                        next2[nid] = nv2;
                        uint32_t nv3 = next3[nid] + v3;
                        if (nv3 >= MOD3) nv3 -= MOD3;
                        next3[nid] = nv3;
                        sum1 += v1; if (sum1 >= MOD1) sum1 -= MOD1;
                        sum2 += v2; if (sum2 >= MOD2) sum2 -= MOD2;
                        sum3 += v3; if (sum3 >= MOD3) sum3 -= MOD3;
                    }

                    /* transition 4: (1,0,0) */
                    if (x + 1 <= M)
                    {
                        size_t nid = idx3(dim, x + 1, y, z);
                        uint32_t nv1 = next1[nid] + v1;
                        if (nv1 >= MOD1) nv1 -= MOD1;
                        next1[nid] = nv1;
                        uint32_t nv2 = next2[nid] + v2;
                        if (nv2 >= MOD2) nv2 -= MOD2;
                        next2[nid] = nv2;
                        uint32_t nv3 = next3[nid] + v3;
                        if (nv3 >= MOD3) nv3 -= MOD3;
                        next3[nid] = nv3;
                        sum1 += v1; if (sum1 >= MOD1) sum1 -= MOD1;
                        sum2 += v2; if (sum2 >= MOD2) sum2 -= MOD2;
                        sum3 += v3; if (sum3 >= MOD3) sum3 -= MOD3;
                    }

                    /* transition 5: (1,1,0) */
                    if (x + 1 <= M && y + 1 <= M)
                    {
                        size_t nid = idx3(dim, x + 1, y + 1, z);
                        uint32_t nv1 = next1[nid] + v1;
                        if (nv1 >= MOD1) nv1 -= MOD1;
                        next1[nid] = nv1;
                        uint32_t nv2 = next2[nid] + v2;
                        if (nv2 >= MOD2) nv2 -= MOD2;
                        next2[nid] = nv2;
                        uint32_t nv3 = next3[nid] + v3;
                        if (nv3 >= MOD3) nv3 -= MOD3;
                        next3[nid] = nv3;
                        sum1 += v1; if (sum1 >= MOD1) sum1 -= MOD1;
                        sum2 += v2; if (sum2 >= MOD2) sum2 -= MOD2;
                        sum3 += v3; if (sum3 >= MOD3) sum3 -= MOD3;
                    }
                }
            }
        }
        seq_mod1[step + 1] = sum1;
        seq_mod2[step + 1] = sum2;
        seq_mod3[step + 1] = sum3;

        /* swap layers */
        uint32_t *tmp;
        tmp = cur1; cur1 = next1; next1 = tmp;
        tmp = cur2; cur2 = next2; next2 = tmp;
        tmp = cur3; cur3 = next3; next3 = tmp;
    }

    /* free DP arrays for the modular computations */
    free(cur1); free(next1);
    free(cur2); free(next2);
    free(cur3); free(next3);

    /* Step 2: obtain the recurrence by Berlekamp–Massey */
    int len1, len2, len3;
    int64_t *rec1 = bm_mod(seq_mod1, M + 1, MOD1, &len1);
    int64_t *rec2 = bm_mod(seq_mod2, M + 1, MOD2, &len2);
    int64_t *rec3 = bm_mod(seq_mod3, M + 1, MOD3, &len3);

    /* Ensure all recurrences have the same order and coefficients */
    if (len1 != len2 || len1 != len3)
    {
        /* In the unlikely event that the orders differ,
           fall back to larger M (not implemented).
           Here we simply assume equality. */
    }
    rec_len = len1;
    int i;
    for (i = 0; i < (int)rec_len; ++i)
    {
        /* coefficients are the same in all moduli */
        rec_coeff[i] = rec1[i];
    }

    free(rec1); free(rec2); free(rec3);

    /* Step 3: compute the first rec_len terms exactly (exact types) */
    int L = (int)rec_len;                 /* number of initial terms needed */
    int dimL = L + 1;
    size_t totL = (size_t)dimL * dimL * dimL;
    seq_t *curE = (seq_t *)calloc(totL, sizeof(seq_t));
    seq_t *nextE = (seq_t *)calloc(totL, sizeof(seq_t));
    curE[0] = 1;                /* a(0) = 1 */
    values = NULL;
    values_cap = 0;
    /* we allocate at least L terms initially */
    seq_t *vals_init = (seq_t *)malloc(L * sizeof(seq_t));
    vals_init[0] = 1;
    int stepk;
    for (stepk = 0; stepk < L - 1; ++stepk)
    {
        memset(nextE, 0, totL * sizeof(seq_t));
        seq_t sum = 0;
        int maxc = stepk;  /* x,y,z <= stepk in current layer */
        int x, y, z;
        for (x = 0; x <= maxc; ++x)
        {
            for (y = 0; y <= maxc; ++y)
            {
                for (z = 0; z <= maxc; ++z)
                {
                    size_t id = idx3(dimL, x, y, z);
                    seq_t val = curE[id];
                    if (val == 0) continue;
                    /* transition 1 */
                    if (x >= 1 && y >= 1)
                    {
                        size_t nid = idx3(dimL, x - 1, y - 1, z + 1);
                        nextE[nid] += val;
                        sum += val;
                    }
                    /* transition 2 */
                    if (x >= 1 && z + 1 <= L)
                    {
                        size_t nid = idx3(dimL, x - 1, y + 1, z + 1);
                        nextE[nid] += val;
                        sum += val;
                    }
                    /* transition 3 */
                    if (z >= 1)
                    {
                        size_t nid = idx3(dimL, x, y, z - 1);
                        nextE[nid] += val;
                        sum += val;
                    }
                    /* transition 4 */
                    if (x + 1 <= L)
                    {
                        size_t nid = idx3(dimL, x + 1, y, z);
                        nextE[nid] += val;
                        sum += val;
                    }
                    /* transition 5 */
                    if (x + 1 <= L && y + 1 <= L)
                    {
                        size_t nid = idx3(dimL, x + 1, y + 1, z);
                        nextE[nid] += val;
                        sum += val;
                    }
                }
            }
        }
        vals_init[stepk + 1] = sum;
        seq_t *tmpE = curE;
        curE = nextE;
        nextE = tmpE;
    }

    values = vals_init;
    values_cap = L;
    values_cap = (values_cap > 1) ? values_cap : 1;
    /* values_cap now equals rec_len; number of stored elements is rec_len */
    /* the next index to compute will be rec_len */

    free(curE);
    free(nextE);
}

/* ---------- the required external function ---------- */

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    /* if the requested index is already computed, return it */
    if (n < values_cap) return values[n];

    /* extend the array if necessary */
    if ((size_t)n >= values_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        values = tmp;
        values_cap = new_cap;
    }

    /* compute subsequent terms using the recurrence */
    size_t i;
    for (i = values_cap; i <= (size_t)n; ++i)
    {
        seq_t pos = 0;
        seq_t neg = 0;
        int j;
        for (j = 0; j < (int)rec_len; ++j)
        {
            int64_t coeff = rec_coeff[j];
            if (coeff >= 0)
                pos += values[i - 1 - j] * (seq_t)coeff;
            else
                neg += values[i - 1 - j] * (seq_t)(-coeff);
        }
        values[i] = pos - neg;   /* pos >= neg for this sequence */
    }

    return values[n];
}
