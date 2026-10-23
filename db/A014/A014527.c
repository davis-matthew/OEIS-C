/*
 * A014527  Inverse of 518th cyclotomic polynomial.
 * https://oeis.org/A014527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the nth coefficient of the power series 1/Φ_518(x) where
 *            Φ_518 is the 518‑th cyclotomic polynomial. The sequence is
 *            periodic with period 518
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The cyclotomic polynomial Φ_518(x) has degree φ(518)=216.
   Its coefficients are computed lazily in init_phi(). */
static seq_t phi[217];
static int phi_initialized = 0;

static void init_phi(void)
{
    const idx_t n     = 518ULL;
    const idx_t maxdeg = 216ULL;               /* φ(518) */
    seq_t tmp[217] = {0};
    tmp[0] = 1;                               /* start with 1 */

    /* Divisors of 518 with μ(518/d) values
       (from the factorisation 518 = 2·7·37).        */
    const idx_t divs[] = {1ULL, 2ULL, 7ULL, 14ULL, 37ULL, 74ULL, 259ULL, 518ULL};
    const int   mu[]   = {-1,  1,   1,   -1,   1,   -1,   -1,   1};

    for (int di = 0; di < 8; ++di)
    {
        idx_t d   = divs[di];
        int   muD = mu[di];

        if (muD == 1)               /* multiply by (x^d - 1)      */
        {
            seq_t newtmp[217] = {0};
            for (idx_t i = 0; i <= maxdeg; ++i)
            {
                seq_t val = tmp[i];
                if (val == 0) continue;
                newtmp[i] -= val;                 /* -1 * coeff */
                if (i + d <= maxdeg) newtmp[i + d] += val; /* +1 * coeff */
            }
            for (idx_t i = 0; i <= maxdeg; ++i) tmp[i] = newtmp[i];
        }
        else if (muD == -1)         /* multiply by (1 - x^d)^-1   */
        {
            seq_t newtmp[217] = {0};
            for (idx_t i = 0; i <= maxdeg; ++i)
            {
                seq_t val = tmp[i];
                if (val == 0) continue;
                for (idx_t k = 0; i + k * d <= maxdeg; ++k)
                    newtmp[i + k * d] += val;   /* add shifted copies */
            }
            for (idx_t i = 0; i <= maxdeg; ++i) tmp[i] = newtmp[i];
        }
    }
    for (idx_t i = 0; i <= maxdeg; ++i) phi[i] = tmp[i];
    phi_initialized = 1;
}

/* Compute the first 518 values of the sequence by recurrence */
static seq_t cache[518];
static int cache_initialized = 0;

static void init_cache(void)
{
    if (!phi_initialized) init_phi();

    const idx_t period  = 518ULL;
    const idx_t degPhi  = 216ULL;

    cache[0] = 1;
    for (idx_t k = 1; k < period; ++k)
    {
        seq_t sum = 0;
        idx_t up  = (k < degPhi ? k : degPhi);
        for (idx_t j = 1; j <= up; ++j)
            sum += phi[j] * cache[k - j];
        cache[k] = -sum;
    }
    cache_initialized = 1;
}

seq_t a(idx_t n)
{
    if (!cache_initialized) init_cache();
    return cache[n % 518ULL];
}
