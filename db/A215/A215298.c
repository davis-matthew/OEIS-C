/*
 * A215298  Number of solid standard Young tableaux of shape [[n,n-2],[2]].
 * https://oeis.org/A215298
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A215298:
   a(n) = 12*(1 - 4*n + 10*n^2 - 8*n^3 + 2*n^4) * (2*n-4)! / ((n-2)! * (n+1)!),  n ≥ 2
   seq_t and idx_t are provided by the build; implementation uses only integer
   arithmetic and the available seq_t operators. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* helper: Legendre exponent of prime p in m! */
static int legendre(idx_t m, idx_t p)
{
    int e = 0;
    while (m)
    {
        m /= p;
        e += (int)m;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n < 2 */

    /* ------------------------------------------------------------ */
    /* Step 1: prime sieve up to 2*n (needed for exponents) */
    idx_t limit = 2 * n;
    if (limit < 2) limit = 2;            /* safety for small n */
    uint8_t *is_comp = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_comp) return (seq_t)0;       /* allocation failure handled silently */
    memset(is_comp, 0, (limit + 1) * sizeof(uint8_t));

    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (!is_comp[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* array of exponent contributions for each prime value (index by prime) */
    int *exps = (int*)calloc(limit + 1, sizeof(int));
    if (!exps)
    {
        free(is_comp);
        return (seq_t)0;
    }

    /* ------------------------------------------------------------ */
    /* Step 2: exponent from the factorial ratio
       e(p) = v_p((2n-4)!) - v_p((n-2)!) - v_p((n+1)!)   */
    idx_t apow    = 2 * n - 4;
    idx_t nminus2 = n - 2;
    idx_t nplus1  = n + 1;

    for (idx_t p = 2; p <= limit; ++p)
    {
        if (!is_comp[p])            /* p is prime */
        {
            int e = legendre(apow,   p)
                  - legendre(nminus2, p)
                  - legendre(nplus1, p);
            exps[p] = e;
        }
    }

    /* ------------------------------------------------------------ */
    /* Step 3: factor the polynomial part: 12 * (1 - 4n + 10n^2 - 8n^3 + 2n^4) */
    seq_t nseq = (seq_t)n;
    seq_t inner = (seq_t)2 * nseq * nseq * nseq * nseq;     /* 2*n^4 */
    inner -= (seq_t)8 * nseq * nseq * nseq;                 /* -8*n^3 */
    inner += (seq_t)10 * nseq * nseq;                       /* +10*n^2 */
    inner -= (seq_t)4 * nseq;                               /* -4*n   */
    inner += (seq_t)1;                                      /* +1     */
    seq_t poly   = inner * (seq_t)12;                       /* 12*inner */

    /* factor poly using the same prime list */
    for (idx_t p = 2; p <= limit; ++p)
    {
        if (!is_comp[p])
        {
            while (poly % p == 0)
            {
                poly /= p;
                exps[p]++;          /* add one more factor of p */
            }
        }
    }

    /* after dividing by all primes ≤ limit we may still have a remainder */
    seq_t remaining = poly;   /* may be 1 or a product of large primes */

    /* ------------------------------------------------------------ */
    /* Step 4: build the result from the accumulated exponents */
    seq_t result = (seq_t)1;
    for (idx_t p = 2; p <= limit; ++p)
    {
        int e = exps[p];
        if (e > 0)
        {
            seq_t sp = (seq_t)p;
            for (int k = 0; k < e; ++k)
                result *= sp;
        }
    }

    /* multiply the leftover remainder (if any) */
    if (remaining > (seq_t)1)
        result *= remaining;

    /* clean up */
    free(is_comp);
    free(exps);

    return result;
}
