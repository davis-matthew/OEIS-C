/*
 * A218301  E.g.f. A(x) satisfies A( x/(exp(x)*cosh(x)) ) =
 *          exp(3*x)*cosh(3*x).
 * https://oeis.org/A218301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3/2 * Sum_{k=0..n} (k+3)^(n-1) * binomial(n,k) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the sequence on demand, memoising all
   intermediate values.  It uses only integer arithmetic and
   the provided big‑integer facilities. */

#include <stdlib.h>

/* External linkage; the types seq_t and idx_t are supplied by the build. */

static seq_t *a_cache = NULL;   /* a(n) values */
static seq_t *binom  = NULL;   /* current binomial row C(n,k) */
static seq_t *pows   = NULL;   /* (k+3)^(current n-1) */
static idx_t  max_n  = 0;       /* highest n that has been computed */

/* Ensure that all arrays have room for indices up to n and that a(n)
   (and all lower a(k)) are correctly computed. */
static void ensure_capacity(idx_t n)
{
    if (a_cache != NULL && n <= max_n)
        return;                     /* already available */

    /* First initialisation (no previous data). */
    if (a_cache == NULL)
    {
        max_n = 0;
        /* Allocate space for a(0), binom(0,0), pows[0] */
        a_cache = (seq_t*)realloc(a_cache, sizeof(seq_t));
        binom   = (seq_t*)realloc(binom,   sizeof(seq_t));
        pows    = (seq_t*)realloc(pows,    sizeof(seq_t));

        a_cache[0] = (seq_t)1;  /* a(0) */
        binom[0]   = (seq_t)1;  /* C(0,0) */
        pows[0]    = (seq_t)1;  /* base value (will not be used for n=0) */
    }

    /* Grow all arrays to size n+1. */
    if (n + 1 > max_n + 1)
    {
        a_cache = (seq_t*)realloc(a_cache, (n+1)*sizeof(seq_t));
        binom   = (seq_t*)realloc(binom,   (n+1)*sizeof(seq_t));
        pows    = (seq_t*)realloc(pows,    (n+1)*sizeof(seq_t));

        /* Initialise newly added entries (value 0) */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            a_cache[i] = (seq_t)0;
            binom[i]   = (seq_t)0;
            pows[i]    = (seq_t)0;
        }
    }

    /* Incrementally build the sequence from max_n+1 up to n. */
    for (idx_t cur = max_n + 1; cur <= n; ++cur)
    {
        /* Update the binomial row C(cur,k) using Pascal's rule. */
        for (idx_t k = cur; k > 0; --k)
            binom[k] = binom[k] + binom[k-1];
        /* binom[0] stays 1 */

        if (cur == 1)
        {
            /* For n = 1 the exponent (n-1) is zero, so all powers are 1. */
            pows[0] = (seq_t)1;
            pows[1] = (seq_t)1;
        }
        else
        {
            /* Update existing powers: multiply by (k+3). */
            for (idx_t k = 0; k < cur; ++k)
                pows[k] = pows[k] * (seq_t)(k + 3);

            /* Compute the new power pows[cur] = (cur+3)^(cur-1). */
            seq_t pw = (seq_t)1;
            for (idx_t t = 0; t < cur - 1; ++t)
                pw = pw * (seq_t)(cur + 3);
            pows[cur] = pw;
        }

        /* Compute the summation S(cur). */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= cur; ++k)
            sum = sum + binom[k] * pows[k];

        /* a(cur) = 3/2 * sum  (exact division by 2). */
        a_cache[cur] = (sum * (seq_t)3) / (seq_t)2;
    }

    max_n = n;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_capacity(n);
    return a_cache[n];
}
