/*
 * A008139  Coordination sequence T2 for Zeolite Code LTL.
 * https://oeis.org/A008139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = (1 + x)^3 * (1 - x + x^2) * (1 + 3*x^2 + x^3 + 2*x^4 +
 *            2*x^5 + 5*x^6 + 2*x^7 + 7*x^8 + 2*x^9 + 5*x^10 + 2*x^11 + 2*x^12
 *            + x^13 + 3*x^14 + x^16) / ((1 - x)^3 * (1 + x^2) * (1 + x^4) *
 *            (1 + x^3 + x^6) * (1 + x + x^2 + x^3 + x^4 + x^5 + x^6)) The
 *            sequence satisfies a linear recurrence of orde...
 * Range:     u8 n <= 11, u16 n <= 187, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static data for the recurrence. */
static long long facQ[22];   /* coefficients of the denominator Q(x)   */
static long long facP[22];   /* coefficients of the numerator  P(x)   */
static seq_t   initA[22];    /* first 22 terms a[0] … a[21]            */
static int initialized = 0;

/* Temporarily multiply two integer polynomials a[0..na-1] and b[0..nb-1].
   Result is put into c[0..na+nb-2].  The caller must provide enough space. */
static void mul_poly(const long long *a, size_t na,
                     const long long *b, size_t nb,
                     long long *c)
{
    size_t i, j;
    for (i = 0; i < na + nb - 1; ++i) c[i] = 0;
    for (i = 0; i < na; ++i) {
        for (j = 0; j < nb; ++j) {
            c[i + j] += a[i] * b[j];
        }
    }
}

/* Build the polynomials Q(x) and P(x) from their factors and compute
   the first 22 terms via the recurrence a(n)+∑q[i]a(n-i)=p(n). */
static void init_sequence(void)
{
    if (initialized) return;

    /* --------------------------------------------
       Denominator Q(x) = (1-x)^3*(1+x^2)*(1+x^4)*(1+x^3+x^6)*(1+x+...+x^6)
       -------------------------------------------- */
    long long Q1[4] = { 1, -3, 3, -1 };          /* (1-x)^3       */
    long long Q2[3] = { 1, 0, 1 };               /* (1+x^2)       */
    long long Q3[5] = { 1, 0, 0, 0, 1 };          /* (1+x^4)       */
    long long Q4[7] = { 1, 0, 0, 1, 0, 0, 1 };    /* (1+x^3+x^6)   */
    long long Q5[7] = { 1, 1, 1, 1, 1, 1, 1 };    /* (1+x+...+x^6) */

    long long tmp1[6], tmp2[10], tmp3[16], Qpoly[22];
    mul_poly(Q1, 4, Q2, 3, tmp1);               /* (1-x)^3 * (1+x^2)          */
    mul_poly(tmp1, 6, Q3, 5, tmp2);             /* + (1+x^4)                  */
    mul_poly(tmp2, 10, Q4, 7, tmp3);            /* + (1+x^3+x^6)              */
    mul_poly(tmp3, 16, Q5, 7, Qpoly);           /* + (1+x+...+x^6)            */
    for (size_t i = 0; i < 22; ++i) facQ[i] = Qpoly[i];

    /* --------------------------------------------
       Numerator P(x) = (1+x)^3*(1-x+x^2)*(1+3x^2+...+x^16)
       -------------------------------------------- */
    long long P1[4] = { 1, 3, 3, 1 };            /* (1+x)^3           */
    long long P2[3] = { 1, -1, 1 };             /* (1-x+x^2)         */
    long long P3[17] = { 1, 0, 3, 1, 2, 2, 5, 2,
                         7, 2, 5, 2, 2, 1, 3, 0, 1 }; /* the polynomial of degree 16 */

    long long tmpP[6], Ppoly[22];
    mul_poly(P1, 4, P2, 3, tmpP);                /* (1+x)^3 * (1-x+x^2)            */
    mul_poly(tmpP, 6, P3, 17, Ppoly);            /* + the degree‑16 polynomial      */
    for (size_t i = 0; i < 22; ++i) facP[i] = Ppoly[i];

    /* --------------------------------------------
       First 22 terms a[0] … a[21] via a(n)+∑q[i]a(n-i)=p(n)
       -------------------------------------------- */
    for (int n = 0; n < 22; ++n) {
        seq_t pos = 0, neg = 0;
        for (int i = 1; i <= n; ++i) {
            long long coef = facQ[i];
            if (coef >= 0)
                pos += initA[n - i] * (seq_t)coef;
            else
                neg += initA[n - i] * (seq_t)(-coef);
        }
        /* a[n] = p[n] - (pos - neg) = p[n] - pos + neg */
        __int128 val = (__int128)facP[n] - (__int128)pos + (__int128)neg;
        initA[n] = (seq_t)val;
    }

    initialized = 1;
}

/* Cache for all computed terms. */
static seq_t  *cache = NULL;   /* array holding a[0..n] */
static size_t cache_len = 0;   /* number of entries stored */

/* Compute all terms up to and including n, extending the cache if necessary. */
static void compute_terms_up_to(idx_t n)
{
    if ((idx_t)cache_len > n) return;     /* already present */

    init_sequence();                      /* ensure init data ready */

    size_t start = cache_len;              /* first index to compute     */
    size_t new_len = (size_t)n + 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                    /* out of memory */
    cache = tmp;

    for (size_t i = start; i < new_len; ++i) {
        if (i <= 21) {
            cache[i] = initA[i];
        } else {
            seq_t pos = 0, neg = 0;
            for (int j = 1; j <= 21; ++j) {
                long long coef = -facQ[j];           /* coefficient in the recurrence a[n] = Σ(-q[j]) a[n-j] */
                if (coef >= 0)
                    pos += cache[i - j] * (seq_t)coef;
                else
                    neg += cache[i - j] * (seq_t)(-coef);
            }
            cache[i] = pos - neg;
        }
    }
    cache_len = new_len;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    init_sequence();                      /* initialize if needed */

    if (n <= 21)                          /* first 22 terms are known */
        return initA[(size_t)n];

    compute_terms_up_to(n);
    return cache[(size_t)n];
}
