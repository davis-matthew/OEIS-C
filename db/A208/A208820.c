/*
 * A208820  Number of n-bead necklaces labeled with numbers -3..3 allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2n)) * ( Σ_{d|n} φ(n/d) * c_d + Reflect(n) ), where
 *            c_d = number of sequences of length d with sum 0 and Reflect(n)
 *            is the sum of fixed necklaces under all reflections (odd and
 *            even handled separately)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requisite types are supplied by the build environment. */
/* idx_t : unsigned integer type for indices (typically uint64_t). */
/* seq_t : unsigned integer type (uint8_t…uint64_t or arbitrary‑precision). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *rot_coef_cache  = NULL;     /* c_d for d = 1..max_n    */
static seq_t (*D_cache)[7]      = NULL;   /* f(m,t) for m ≤ max_n/2   */
static uint64_t *phi_cache      = NULL;   /* φ(v) for v ≤ max_n       */
static size_t precomp_n         = 0;       /* largest n processed      */

/* ------------------------------------------------------------------ */
/* Pre‑compute rotation coefficients, diagonal counts and the totient
   function up to the largest n requested so far.  All arrays are
   resized when a larger n is needed. */
static void precompute(idx_t max_n)
{
    if (max_n <= precomp_n) return;        /* already up to date */

    /* -------- Euler totient φ[0..max_n] -------- */
    size_t phi_cap = (size_t)max_n + 1;
    uint64_t *new_phi = (uint64_t *)realloc(phi_cache, phi_cap * sizeof(uint64_t));
    if (!new_phi) return;                  /* malloc failure – caller must abort */
    phi_cache = new_phi;
    for (size_t i = 0; i < phi_cap; ++i) phi_cache[i] = i;
    for (size_t i = 2; i <= max_n; ++i) {
        if (phi_cache[i] == i) {           /* i is prime */
            for (size_t j = i; j <= max_n; j += i)
                phi_cache[j] -= phi_cache[j] / i;
        }
    }

    /* -------- Rotation coefficient cache (c_d) -------- */
    size_t rot_cap = (size_t)max_n + 1;
    seq_t *new_rot = (seq_t *)realloc(rot_coef_cache, rot_cap * sizeof(seq_t));
    if (!new_rot) return;
    rot_coef_cache = new_rot;
    rot_coef_cache[0] = (seq_t)0;          /* unused */

    /* -------- Diagonal count cache (f(m,t)) -------- */
    size_t max_m = max_n / 2;
    size_t d_rows = (size_t)max_m + 1;
    seq_t (*new_D)[7] = (seq_t (*)[7])realloc(D_cache,
                                               d_rows * sizeof(seq_t[7]));
    if (!new_D) return;
    D_cache = new_D;
    for (size_t r = 0; r < d_rows; ++r)
        for (int t = 0; t < 7; ++t)
            D_cache[r][t] = (seq_t)0;

    /* -------- DP arrays for the generating function (1+x+…+x^6)^m -------- */
    size_t max_range = 6 * max_n + 1;     /* indices for sums -3m..+3m */
    seq_t *dp_prev = (seq_t *)calloc(max_range, sizeof(seq_t));
    seq_t *dp_cur  = (seq_t *)calloc(max_range, sizeof(seq_t));
    if (!dp_prev || !dp_cur) return;
    dp_prev[0] = (seq_t)1;               /* m = 0 : one empty sequence with sum 0 */

    /* ---- Store data for m = 0 ---- */
    for (int t = 0; t < 7; ++t)
        D_cache[0][t] = (t == 3) ? (seq_t)1 : (seq_t)0;

    /* ---- Incrementally compute up to m = max_n ---- */
    for (size_t m = 1; m <= max_n; ++m) {
        size_t range = 6 * m + 1;         /* dp indices 0 .. 6m */
        for (size_t k = 0; k < range; ++k) {
            seq_t s = (seq_t)0;
            for (size_t i = 0; i <= 6; ++i) {
                if (k >= i) s += dp_prev[k - i];
            }
            dp_cur[k] = s;
        }
        /* Save zero‑sum coefficient c_m */
        rot_coef_cache[(idx_t)m] = dp_cur[3 * m];

        /* Store diagonal counts f(m,t) for |t|≤3 only when needed */
        if (m <= max_m) {
            for (int t = 0; t < 7; ++t) {   /* t = -3 … +3 */
                size_t idx = 3 * m + t - 3;  /* actual index for sum t */
                D_cache[(idx_t)m][t] = dp_cur[idx];
            }
        }

        /* Rotate the DP buffers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur = tmp;
    }

    free(dp_prev);
    free(dp_cur);
    precomp_n = max_n;
}

/* ------------------------------------------------------------------ */
/* Helper arrays for diagonal counts of pairs and their sums. */
static const int pair_count[7] = {1,3,5,7,5,3,1};
static const int E_vals[7]    = {-6,-4,-2,0,2,4,6};

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    precompute(n);                         /* ensure super‑arrays ready */

    /* ---------- Rotations ---------- */
    seq_t sum_rot = (seq_t)0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;
        seq_t coef1 = rot_coef_cache[(idx_t)d1];
        seq_t phi1  = (seq_t)phi_cache[(size_t)(n / d1)];
        sum_rot += phi1 * coef1;
        if (d1 != d2) {
            seq_t coef2 = rot_coef_cache[(idx_t)d2];
            seq_t phi2  = (seq_t)phi_cache[(size_t)(n / d2)];
            sum_rot += phi2 * coef2;
        }
    }

    /* ---------- Reflections ---------- */
    seq_t sum_ref = (seq_t)0;
    if (n & 1) {                           /* n odd */
        idx_t m = (n - 1) / 2;             /* number of 2‑cycles */
        seq_t local = (seq_t)0;
        /* Values for the fixed bead: -2, 0, 2 */
        for (int idx = 0; idx < 3; ++idx) {
            int v0 = -2 + 2 * idx;         /* -2, 0, 2 */
            int target = -v0 / 2;          /* -(-2)/2 = 1, etc. */
            local += D_cache[(idx_t)m][target + 3];
        }
        sum_ref = local * (seq_t)n;
    } else {                               /* n even */
        /* Type A: two fixed beads */
        idx_t mA = (n - 2) / 2;
        seq_t termA = (seq_t)0;
        for (int idx = 0; idx < 7; ++idx) {
            int E = E_vals[idx];           /* sum of the two fixed beads */
            int target = -E / 2;           /* required sum of pair values */
            termA += (seq_t)pair_count[idx] * D_cache[(idx_t)mA][target + 3];
        }
        termA *= (seq_t)(n / 2);

        /* Type B: no fixed beads */
        idx_t mB = n / 2;
        seq_t termB = D_cache[(idx_t)mB][3];   /* target 0 -> index 3 */
        termB *= (seq_t)(n / 2);

        sum_ref = termA + termB;
    }

    /* ---------- Burnside ---------- */
    seq_t total = sum_rot + sum_ref;
    seq_t denom = (seq_t)(2 * n);
    return total / denom;
}
