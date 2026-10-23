/*
 * A033616  Coordination sequence T1 for Zeolite Code TSC.
 * https://oeis.org/A033616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A033616 (Coordination sequence T1 for Zeolite Code TSC).
   The generating function is a rational GF.  Let
        Q(x) = (1 - x)^3 * (1 - x + x^2 - x^3 + x^4)
                * (1 + x + x^2 + x^3 + x^4)
                * (1 + x^3 + x^6)
                * (1 + x + x^2 + x^3 + x^4 + x^5 + x^6)
          = 1 + Σ_{i=1}^{23} q_i x^i
        P(x) = (1 + x)^3 * (1 + x^2) *
              (1 - x + 2 x^2 - x^3 + 3 x^4 - x^5 + 4 x^6 - … + x^18)
          = Σ_{i=0}^{23} p_i x^i
   Then for n ≥ 0
        a[n] = p[n] - Σ_{i=1}^{min(n,23)} q_i · a[n-i]
   (with p[n] = 0 for n > 23).  This recurrence is O(n) once the first
   24 terms are available.  Initial values are produced automatically
   by the recurrence with p[n] for n ≤ 23. */

#include <stdlib.h>
#include <stdbool.h>

static void poly_mul(const long long *a, size_t la,
                     const long long *b, size_t lb,
                     long long *res)
{
    size_t lr = la + lb - 1;
    for (size_t i = 0; i < lr; ++i) res[i] = 0;
    for (size_t i = 0; i < la; ++i)
        for (size_t j = 0; j < lb; ++j)
            res[i + j] += a[i] * b[j];
}

/* initialise the coefficient tables Q[24] and P[24] */
static void init_coeffs(long long **q, long long **p)
{
    static bool done = false;
    static long long *Q = NULL;
    static long long *P = NULL;

    if (done) { *q = Q; *p = P; return; }

    /* temporary buffer (max degree 23+1 = 24) */
    long long t[30];

    /* compute Q = (1 - x)^3 * (1 - x + x^2 - x^3 + x^4)
                       * (1 + x + x^2 + x^3 + x^4)
                       * (1 + x^3 + x^6)
                       * (1 + x + x^2 + x^3 + x^4 + x^5 + x^6) */
    const long long Q1[4] = { 1, -3, 3, -1 };              /* (1 - x)^3          */
    const long long Q2[5] = { 1, -1, 1, -1, 1 };           /* 1 - x + x^2 - x^3 + x^4 */
    const long long Q3[5] = { 1, 1, 1, 1, 1 };             /* 1 + x + x^2 + x^3 + x^4 */
    const long long Q4[7] = { 1, 0, 0, 1, 0, 0, 1 };       /* 1 + x^3 + x^6       */
    const long long Q5[7] = { 1, 1, 1, 1, 1, 1, 1 };       /* 1 + x + … + x^6     */

    poly_mul(Q1, 4, Q2, 5, t);              /* Q1 * Q2 -> t, len 8 */
    long long tmp2[30];
    poly_mul(t, 8, Q3, 5, tmp2);            /* * Q3 -> tmp2, len 12 */
    poly_mul(tmp2, 12, Q4, 7, t);           /* * Q4 -> t,   len 18 */
    poly_mul(t, 18, Q5, 7, tmp2);           /* * Q5 -> tmp2, len 24 */

    Q = (long long*)malloc(24 * sizeof(long long));
    for (int i = 0; i < 24; ++i) Q[i] = tmp2[i];

    /* compute P = (1 + x)^3 * (1 + x^2) *
                     (1 - x + 2x^2 - x^3 + 3x^4 - ... + x^18) */
    const long long R[19] = { 1, -1, 2, -1, 3, -1, 4, -1,
                              4, -1, 4, -1, 4, -1, 3,
                              -1, 2, -1, 1 };               /* degree 18 */
    const long long B0[3] = { 1, 0, 1 };                 /* 1 + x^2          */
    const long long B1[4] = { 1, 3, 3, 1 };              /* (1 + x)^3        */

    poly_mul(R, 19, B0, 3, t);               /* R * B0 -> t, len 21 */
    poly_mul(t, 21, B1, 4, tmp2);           /* * B1 -> tmp2, len 24 */

    P = (long long*)malloc(24 * sizeof(long long));
    for (int i = 0; i < 24; ++i) P[i] = tmp2[i];

    *q = Q;
    *p = P;
    done = true;
}

seq_t a(idx_t n)
{
    /* static storage */
    static long long *Qarr = NULL;
    static long long *Parr = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;
    static seq_t *cache = NULL;

    const int K = 23;  /* recurrence order */

    /* initialise coefficients once */
    if (!Qarr || !Parr) init_coeffs(&Qarr, &Parr);

    /* ensure capacity */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= (idx_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        long long sum = 0;
        int maxk = (i < K) ? (int)i : K;   /* only up to available terms */
        for (int j = 1; j <= maxk; ++j)
            sum += Qarr[j] * (long long)cache[i - j];

        long long val;
        if (i <= K) val = Parr[i] - sum;
        else        val = -sum;            /* p[i] = 0 for i > 23 */

        cache[i] = (seq_t)val;
    }
    cache_size = n + 1;
    return cache[n];
}
