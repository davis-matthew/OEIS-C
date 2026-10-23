/*
 * A151283  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0) and consisting of n steps taken from {(-1, 0), (0, 1), (1,
 *          -1), (1, 0)}.
 * https://oeis.org/A151283
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

/* OEIS A151283: Number of walks within N^2 (first quadrant)
   starting at (0,0) with steps {(-1,0),(0,1),(1,-1),(1,0)}. */
/* The sequence satisfies a linear recurrence with constant coefficients.
   The recurrence is recovered from the first 200 terms using Berlekamp–Massey
   modulo two large primes, then lifted to integers by CRT.  All subsequent
   terms are generated with that recurrence. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INIT   200         /* number of terms used to discover recurrence */
#define PRIME1    1000000007LL /* first prime for BM */
#define PRIME2    1000000009LL /* second prime for BM */
#define MAX_ORDER 200          /* maximum possible order for safety */

/* ---------- modular helpers ---------- */
static int64_t mod_pow(int64_t a, int64_t e, int64_t m)
{
    int64_t r = 1 % m;
    a %= m;
    while (e > 0) {
        if (e & 1) r = (__int128)r * a % m;
        a = (__int128)a * a % m;
        e >>= 1;
    }
    return r;
}

static int64_t mod_inv(int64_t a, int64_t m)
{
    return mod_pow(a, m - 2, m);
}

static int64_t crt_two(int64_t r1, int64_t r2,
                       int64_t m1, int64_t m2)
{
    /* Chinese remainder theorem for two coprime moduli */
    int64_t inv = mod_inv(m1 % m2, m2);
    int64_t t = r2 - r1;
    t %= m2;
    if (t < 0) t += m2;
    t = t * inv % m2;
    return r1 + m1 * t; /* value in [0, m1*m2) */
}

/* ---------- Berlekamp–Massey ---------- */
static void bm(const int64_t *s, size_t N, int64_t mod,
               int64_t *coeff, size_t *outLen)
{
    static int64_t C[MAX_ORDER + 1];
    static int64_t B[MAX_ORDER + 1];
    size_t L = 0;
    int64_t b = 1;
    size_t m = 1;

    memset(C, 0, sizeof(C));
    memset(B, 0, sizeof(B));
    C[0] = 1;
    B[0] = 1;

    for (size_t n = 0; n < N; ++n) {
        int64_t d = 0;
        for (size_t i = 0; i <= L; ++i) {
            d = (d + C[i] * s[n - i]) % mod;
        }
        if (d < 0) d += mod;
        if (d == 0) {
            ++m;
            continue;
        }
        int64_t coef = d * mod_inv(b, mod) % mod;
        static int64_t T[MAX_ORDER + 1];
        memcpy(T, C, (L + 1) * sizeof(int64_t));

        if (m + L > L) {
            for (size_t i = L + 1; i <= L + m; ++i) C[i] = 0;
        }
        for (size_t i = 0; i <= L; ++i) {
            C[i + m] = (C[i + m] - coef * B[i]) % mod;
            if (C[i + m] < 0) C[i + m] += mod;
        }
        if (2 * L <= n) {
            L = n + 1 - L;
            memcpy(B, T, (L + 1) * sizeof(int64_t));
            b = d;
            m = 1;
        } else {
            ++m;
        }
    }
    *outLen = L;
    for (size_t i = 0; i < L; ++i) coeff[i] = C[i + 1];
}

/* ---------- DP to generate first terms ---------- */
static void compute_terms(seq_t *out, size_t N)
{
    static seq_t dp_cur[MAX_INIT + 1][MAX_INIT + 1];
    static seq_t dp_nxt[MAX_INIT + 1][MAX_INIT + 1];
    size_t i, j, step;

    memset(dp_cur, 0, sizeof(dp_cur));
    dp_cur[0][0] = 1;
    out[0] = 1;

    for (step = 0; step < N; ++step) {
        for (i = 0; i <= N; ++i)
            memset(dp_nxt[i], 0, (N + 1) * sizeof(seq_t));

        for (j = 0; j <= step; ++j) {
            for (i = 0; i <= step; ++i) {
                seq_t val = dp_cur[j][i];
                if (!val) continue;
                if (i > 0) dp_nxt[j][i - 1] += val;   /* left   */
                dp_nxt[j + 1][i] += val;               /* up     */
                dp_nxt[j][i + 1] += val;               /* right  */
                if (j > 0) dp_nxt[j - 1][i + 1] += val;/* down‑right */
            }
        }

        seq_t total = 0;
        for (j = 0; j <= step + 1; ++j)
            for (i = 0; i <= step + 1; ++i)
                total += dp_nxt[j][i];
        out[step + 1] = total;

        for (i = 0; i <= N; ++i)
            memcpy(dp_cur[i], dp_nxt[i], (N + 1) * sizeof(seq_t));
    }
}

static void compute_mod_terms(int64_t *out, size_t N, int64_t mod)
{
    static int64_t dp_cur[MAX_INIT + 1][MAX_INIT + 1];
    static int64_t dp_nxt[MAX_INIT + 1][MAX_INIT + 1];
    size_t i, j, step;

    memset(dp_cur, 0, sizeof(dp_cur));
    dp_cur[0][0] = 1 % mod;
    out[0] = 1 % mod;

    for (step = 0; step < N; ++step) {
        for (i = 0; i <= N; ++i)
            memset(dp_nxt[i], 0, (N + 1) * sizeof(int64_t));

        for (j = 0; j <= step; ++j) {
            for (i = 0; i <= step; ++i) {
                int64_t val = dp_cur[j][i];
                if (!val) continue;
                if (i > 0) {
                    dp_nxt[j][i - 1] += val;
                    if (dp_nxt[j][i - 1] >= mod) dp_nxt[j][i - 1] -= mod;
                }
                dp_nxt[j + 1][i] += val;
                if (dp_nxt[j + 1][i] >= mod) dp_nxt[j + 1][i] -= mod;
                dp_nxt[j][i + 1] += val;
                if (dp_nxt[j][i + 1] >= mod) dp_nxt[j][i + 1] -= mod;
                if (j > 0) {
                    dp_nxt[j - 1][i + 1] += val;
                    if (dp_nxt[j - 1][i + 1] >= mod) dp_nxt[j - 1][i + 1] -= mod;
                }
            }
        }

        int64_t total = 0;
        for (j = 0; j <= step + 1; ++j)
            for (i = 0; i <= step + 1; ++i) {
                total += dp_nxt[j][i];
                if (total >= mod) total -= mod;
            }
        out[step + 1] = total;

        for (i = 0; i <= N; ++i)
            memcpy(dp_cur[i], dp_nxt[i], (N + 1) * sizeof(int64_t));
    }
}

/* ---------- global state ---------- */
static bool initialized = false;
static seq_t *terms = NULL;       /* cached terms */
static size_t terms_size = 0;
static int64_t *rec_coeff = NULL; /* recurrence coefficients (signed) */
static size_t rec_order = 0;

/* ---------- initialization ---------- */
static void init(void)
{
    if (initialized) return;

    /* 1. Compute first MAX_INIT terms */
    seq_t init_vals[MAX_INIT + 1];
    compute_terms(init_vals, MAX_INIT);

    /* 2. Compute same terms modulo two primes */
    int64_t mod1[MAX_INIT + 1];
    int64_t mod2[MAX_INIT + 1];
    compute_mod_terms(mod1, MAX_INIT, PRIME1);
    compute_mod_terms(mod2, MAX_INIT, PRIME2);

    /* 3. Find minimal linear recurrence via BM */
    int64_t coeff1[MAX_ORDER];
    int64_t coeff2[MAX_ORDER];
    size_t len1, len2;
    bm(mod1, MAX_INIT + 1, PRIME1, coeff1, &len1);
    bm(mod2, MAX_INIT + 1, PRIME2, coeff2, &len2);

    if (len1 != len2) rec_order = (len1 < len2) ? len1 : len2;
    else rec_order = len1;

    rec_coeff = (int64_t *)malloc(rec_order * sizeof(int64_t));
    int64_t modprod = PRIME1 * PRIME2;
    for (size_t i = 0; i < rec_order; ++i) {
        int64_t a1 = coeff1[i];
        int64_t a2 = coeff2[i];
        int64_t merged = crt_two(a1, a2, PRIME1, PRIME2);
        /* convert to signed integer in (-modprod/2, modprod/2] */
        if (merged > modprod / 2) merged -= modprod;
        rec_coeff[i] = merged;
    }

    /* 4. Store initial terms and allocate array */
    size_t cap = (MAX_INIT + 1);
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    for (size_t i = 0; i <= MAX_INIT; ++i) terms[i] = init_vals[i];
    terms_size = MAX_INIT + 1;

    initialized = true;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    init();

    if ((size_t)n < terms_size) return terms[(size_t)n];

    /* compute further terms using the recurrence */
    for (idx_t k = terms_size; k <= n; ++k) {
        seq_t val = 0;
        for (size_t i = 0; i < rec_order; ++i) {
            seq_t v = terms[(size_t)(k - 1 - i)];
            if (rec_coeff[i] >= 0)
                val += v * (seq_t)rec_coeff[i];
            else
                val -= v * (seq_t)(-rec_coeff[i]);
        }
        terms = (seq_t *)realloc(terms, (k + 1) * sizeof(seq_t));
        terms[k] = val;
    }
    terms_size = (size_t)n + 1;
    return terms[(size_t)n];
}
