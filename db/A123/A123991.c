/*
 * A123991  Expansion of o.g.f. (1-x^2+x^4)/((1-x)^2*(1-x^2)^4*(1-x^3)^4).
 * https://oeis.org/A123991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^n in (1 - x^2 + x^4) / ((1 - x)^2 *
 *            (1 - x^2)^4 * (1 - x^3)^4)
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 98, u64 n <= 1262,
 *            u128 n <= 9999, big any n
 * Helpers:   binom, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"          /* for binom(idx_t a, idx_t b) */
#include "../utils/seq_alloc_new_array.h"   /* for seq_alloc_new_array(size_t) */

static int  coeff[23];   /* denominator polynomial coefficients d[0..22] */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t  cached_upto = 0;
static int    initialized = 0;

/* ------------------------------------------------------------------ */
/*  Compute the coefficients of the denominator polynomial
    D(x) = (1-x)^2 * (1-x^2)^4 * (1-x^3)^4
    The coefficients are stored in coeff[0..22] (coeff[0] == 1)         */
static void compute_coeffs(void)
{
    int q[9]   = {0};   /* (1-x^2)^4 coefficients 0..8   */
    int r[13]  = {0};   /* (1-x^3)^4 coefficients 0..12  */
    int g[3]   = {1, -2, 1}; /* (1-x)^2 coefficients 0..2    */
    int d[22]  = {0};   /* intermediate product q*r (deg <=20) */
    int p[23]  = {0};   /* final polynomial (deg <=22)       */

    /* (1-x^2)^4 */
    q[0] =  1;
    q[2] = -4;
    q[4] =  6;
    q[6] = -4;
    q[8] =  1;

    /* (1-x^3)^4 */
    r[0] =  1;
    r[3] = -4;
    r[6] =  6;
    r[9] = -4;
    r[12] = 1;

    /* d = q * r */
    for (int i = 0; i <= 8; ++i)
        if (q[i])
            for (int j = 0; j <= 12; ++j)
                if (r[j])
                    d[i + j] += q[i] * r[j];

    /* p = d * g = (1-x)^2 * (1-x^2)^4 * (1-x^3)^4 */
    for (int i = 0; i <= 20; ++i)
        for (int j = 0; j <= 2; ++j)
            if (g[j])
                p[i + j] += d[i] * g[j];

    /* store coefficients */
    for (int k = 0; k <= 22; ++k)
        coeff[k] = p[k];   /* coeff[0] is 1 */
}

/* ------------------------------------------------------------------ */
/*  Compute the first terms a(0)..a(21) directly from the generating
    function expansion.                                          */
static void compute_first_terms(void)
{
    const idx_t N = 21;                      /* we need terms up to a[21] */
    seq_t f1[N + 1];   /* 1/(1-x)^2  -> coefficient t+1               */
    seq_t f2[N + 1];   /* 1/(1-x^2)^4 -> binom(3+t, t) at index 2*t   */
    seq_t f3[N + 1];   /* 1/(1-x^3)^4 -> binom(3+t, t) at index 3*t   */
    seq_t conv12[N + 1];
    seq_t conv123[N + 1];

    /* 1/(1-x)^2 */
    for (idx_t t = 0; t <= N; ++t)
        f1[t] = (seq_t)(t + 1);

    /* 1/(1-x^2)^4 */
    for (idx_t t = 0; t <= N; ++t)
        f2[t] = 0;
    for (idx_t t = 0; 2 * t <= N; ++t)
        f2[2 * t] = binom(3 + t, t);

    /* 1/(1-x^3)^4 */
    for (idx_t t = 0; t <= N; ++t)
        f3[t] = 0;
    for (idx_t t = 0; 3 * t <= N; ++t)
        f3[3 * t] = binom(3 + t, t);

    /* conv12 = f1 * f2 */
    for (idx_t i = 0; i <= N; ++i) {
        conv12[i] = 0;
        for (idx_t j = 0; j <= i; ++j)
            conv12[i] += f1[j] * f2[i - j];
    }

    /* conv123 = conv12 * f3 */
    for (idx_t i = 0; i <= N; ++i) {
        conv123[i] = 0;
        for (idx_t j = 0; j <= i; ++j)
            conv123[i] += conv12[j] * f3[i - j];
    }

    /* Multiply by numerator 1 - x^2 + x^4
       a[n] = conv123[n] - conv123[n-2] + conv123[n-4]   */
    for (idx_t n = 0; n <= N; ++n) {
        seq_t s0 = conv123[n];
        seq_t s2 = 0;
        seq_t s4 = 0;
        if (n >= 2) s2 = conv123[n - 2];
        if (n >= 4) s4 = conv123[n - 4];
        cache[n] = s0 + s4 - s2;
    }

    cached_upto = N;   /* we have computed terms up to a[21] */
}

/* ------------------------------------------------------------------ */
/*  Initialization routine: fill coeff[], allocate cache, compute
    first 22 terms.                                                */
static void init_sequence(void)
{
    compute_coeffs();

    /* Allocate cache capable of holding at least 20010 terms
       (enough for n up to about 10000 with ample headroom). */
    cache_size = 20010;
    cache = seq_alloc_new_array(cache_size);
    for (size_t i = 0; i < cache_size; ++i)
        cache[i] = 0;          /* zero-initialize */

    compute_first_terms();
    initialized = 1;
}

/* ------------------------------------------------------------------ */
/*  Main function required by the exercise.                          */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    if (n > cached_upto) {
        /* compute missing terms using the linear recurrence
           ∑_{k=0}^{22} coeff[k] * a[n-k] = 0  (for n ≥ 5)          */
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            seq_t pos_sum = 0;   /* sum of coeff[k] * a[i-k] for coeff[k] > 0 */
            seq_t neg_sum = 0;   /* sum of |coeff[k]| * a[i-k] for coeff[k] < 0 */

            for (int k = 1; k <= 22; ++k) {
                const int ck = coeff[k];
                if (ck == 0)
                    continue;
                const seq_t val = cache[i - k];
                if (ck > 0)
                    pos_sum += (seq_t)ck * val;
                else /* ck < 0 */
                    neg_sum += (seq_t)(-ck) * val;   /* absolute value   */
            }
            cache[i] = neg_sum - pos_sum;  /* γ_i = 0 for i ≥ 5          */
        }
        cached_upto = n;
    }
    return cache[n];
}
