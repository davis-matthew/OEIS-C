/*
 * A013311  arcsin(exp(x)-cos(x))=x+2/2!*x^2+2/3!*x^3+12/4!*x^4+80/5!*x^5...
 * https://oeis.org/A013311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * coefficient of x^n in arcsin(exp(x)-cos(x))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses scaled integer arithmetic (EGF basis)        */
#include <stdlib.h>
#include <stdint.h>

static seq_t *fSc   = NULL;   /* f(x) scaled coefficients (f_i = coeff * i!)  */
static seq_t *v     = NULL;   /* (f^2) scaled coefficients                  */
static seq_t *b     = NULL;   /* (1/(1-f^2)) scaled coefficients           */
static seq_t *gG    = NULL;   /* sqrt(b) scaled coefficients (G(x))         */
static seq_t *aVals = NULL;   /* desired a(n) values                        */
static idx_t  computed = 0;   /* highest n that has been pre‑computed        */

/* Ensure all values up to n are available */
static void ensure(idx_t n)
{
    if (n <= computed) return;

    idx_t newSize = n;
    size_t sz = (newSize + 1) * sizeof(seq_t);

    fSc   = (seq_t *)realloc(fSc  , sz);
    v     = (seq_t *)realloc(v    , sz);
    b     = (seq_t *)realloc(b    , sz);
    gG    = (seq_t *)realloc(gG   , sz);
    aVals = (seq_t *)realloc(aVals, sz);

    /* initialise fSc for new indices */
    for (idx_t i = computed + 1; i <= newSize; ++i)
    {
        if ((i & 3) == 0)          fSc[i] = 0; /* multiple of 4 */
        else if ((i & 1) == 0)      fSc[i] = 2; /* even but not 4k   */
        else                       fSc[i] = 1; /* odd               */
    }

    /* compute (f^2) scaled coefficients v[i] for new indices */
    for (idx_t k = computed + 1; k <= newSize; ++k)
    {
        seq_t sum  = 0;
        seq_t binom = 1; /* binom(k,0) */
        for (idx_t i = 0; i <= k; ++i)
        {
            seq_t prod = fSc[i] * fSc[k-i];
            sum += prod * binom;
            if (i < k)
                binom = (binom * (k-i)) / (i+1);
        }
        v[k] = sum;
    }

    /* compute b[n] (scaled 1/(1-f^2)) */
    if (computed == 0) b[0] = 1;
    for (idx_t nn = computed + 1; nn <= newSize; ++nn)
    {
        seq_t sum  = 0;
        seq_t binom = nn;   /* binom(nn,1) */
        for (idx_t k = 1; k <= nn; ++k)
        {
            seq_t term = v[k] * b[nn-k] * binom;
            sum += term;
            if (k < nn)
                binom = (binom * (nn-k)) / (k+1);
        }
        b[nn] = sum;
    }

    /* compute gG[n] (scaled sqrt(b)) */
    if (computed == 0) gG[0] = 1;
    for (idx_t nn = computed + 1; nn <= newSize; ++nn)
    {
        seq_t sum = 0;
        if (nn > 1)
        {
            seq_t binom = nn;  /* binom(nn,1) */
            for (idx_t i = 1; i <= nn-1; ++i)
            {
                seq_t term = binom * gG[i] * gG[nn-i];
                sum += term;
                if (i < nn-1)
                    binom = (binom * (nn-i)) / (i+1);
            }
        }
        gG[nn] = (b[nn] - sum) / 2;   /* must be even */
    }

    /* compute a[n] */
    for (idx_t nn = computed + 1; nn <= newSize; ++nn)
    {
        seq_t sum  = 0;
        seq_t binom = 1;   /* binom(nn-1,0) */
        for (idx_t i = 0; i <= nn-1; ++i)
        {
            seq_t term = binom * fSc[i+1] * gG[nn-1-i];
            sum += term;
            if (i < nn-1)
                binom = (binom * ((nn-1)-i)) / (i+1);
        }
        aVals[nn] = sum;
    }

    computed = newSize;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure(n);
    return aVals[n];
}
