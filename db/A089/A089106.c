/*
 * A089106  Sign twisted convoluted convolved Fibonacci numbers H_j^(10).
 * https://oeis.org/A089106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/10) * coeff_{n-1} [ (1/(1-z-z^2)^10 + 1/(1-z^2-z^4)^5
 *            - 1/(1-z^5-z^10)^2 - 1/(1-z^10-z^20)) ]
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static long long binom(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    long long r = 1;
    for (int i = 1; i <= k; ++i)
        r = r * (n - i + 1) / i;
    return r;
}

static void build_coefficients(long long *coef)
{
    /* compute (1 - z - z^2)^10 */
    for (int i = 0; i <= 20; ++i) coef[i] = 0;
    for (int k = 0; k <= 10; ++k)
    {
        long long c = binom(10, k);
        if (k & 1) c = -c;                  /* (-1)^k */
        for (int j = 0; j <= k; ++j)
        {
            int e = k + j;                  /* exponent */
            long long add = c * binom(k, j);
            coef[e] += add;
        }
    }
}

/* similar functions for the other polynomials */
static void build_coefficients_B(long long *coef)
{
    /* (1 - z^2 - z^4)^5 */
    for (int i = 0; i <= 20; ++i) coef[i] = 0;
    for (int k = 0; k <= 5; ++k)
    {
        long long c = binom(5, k);
        if (k & 1) c = -c;                  /* (-1)^k */
        for (int j = 0; j <= k; ++j)
        {
            int e = 2 * (k + j);            /* z^2 and z^4 => 2*(k+j) */
            long long add = c * binom(k, j);
            coef[e] += add;
        }
    }
}

static void build_coefficients_C(long long *coef)
{
    /* (1 - z^5 - z^10)^2 */
    for (int i = 0; i <= 20; ++i) coef[i] = 0;
    for (int k = 0; k <= 2; ++k)
    {
        long long c = binom(2, k);
        if (k & 1) c = -c;                  /* (-1)^k */
        for (int j = 0; j <= k; ++j)
        {
            int e = 5 * (k + j);            /* exponent multiples of 5 */
            long long add = c * binom(k, j);
            coef[e] += add;
        }
    }
}

static void build_coefficients_D(long long *coef)
{
    /* (1 - z^10 - z^20)^1 */
    for (int i = 0; i <= 20; ++i) coef[i] = 0;
    for (int k = 0; k <= 1; ++k)
    {
        long long c = binom(1, k);
        if (k & 1) c = -c;                  /* (-1)^k */
        for (int j = 0; j <= k; ++j)
        {
            int e = 10 * (k + j);           /* exponent multiples of 10 */
            long long add = c * binom(k, j);
            coef[e] += add;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but return 0 */
        return 0;

    /* coefficients for the four rational functions */
    static long long coeffA[21], coeffB[21], coeffC[21], coeffD[21];
    static int initialized = 0;
    if (!initialized)
    {
        build_coefficients(coeffA);
        build_coefficients_B(coeffB);
        build_coefficients_C(coeffC);
        build_coefficients_D(coeffD);
        initialized = 1;
    }

    /* allocate arrays for the series up to index n-1 */
    idx_t size = n;                      /* we need indices 0 .. n-1 */
    seq_t *aA = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *aB = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *aC = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *aD = (seq_t*)calloc(size, sizeof(seq_t));

    if (!aA || !aB || !aC || !aD) return 0;  /* allocation failure */

    aA[0] = aB[0] = aC[0] = aD[0] = (seq_t)1;   /* constant term is 1 */

    for (idx_t i = 1; i < size; ++i)
    {
        /* compute aA[i] */
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 20; ++k)
        {
            long long c = coeffA[k];
            if (c == 0 || i < (idx_t)k) continue;
            seq_t term = (seq_t)(c > 0 ? c : -c) * aA[i - k];
            if (c > 0) pos += term; else neg += term;
        }
        aA[i] = neg - pos;

        /* compute aB[i] */
        pos = neg = 0;
        for (int k = 1; k <= 20; ++k)
        {
            long long c = coeffB[k];
            if (c == 0 || i < (idx_t)k) continue;
            seq_t term = (seq_t)(c > 0 ? c : -c) * aB[i - k];
            if (c > 0) pos += term; else neg += term;
        }
        aB[i] = neg - pos;

        /* compute aC[i] */
        pos = neg = 0;
        for (int k = 1; k <= 20; ++k)
        {
            long long c = coeffC[k];
            if (c == 0 || i < (idx_t)k) continue;
            seq_t term = (seq_t)(c > 0 ? c : -c) * aC[i - k];
            if (c > 0) pos += term; else neg += term;
        }
        aC[i] = neg - pos;

        /* compute aD[i] */
        pos = neg = 0;
        for (int k = 1; k <= 20; ++k)
        {
            long long c = coeffD[k];
            if (c == 0 || i < (idx_t)k) continue;
            seq_t term = (seq_t)(c > 0 ? c : -c) * aD[i - k];
            if (c > 0) pos += term; else neg += term;
        }
        aD[i] = neg - pos;
    }

    /* numerator for a(n) */
    idx_t idx = n - 1;
    seq_t numer = aA[idx] + aB[idx];
    numer -= aC[idx];
    numer -= aD[idx];

    seq_t result = numer / (seq_t)10;

    free(aA);
    free(aB);
    free(aC);
    free(aD);

    return result;
}
