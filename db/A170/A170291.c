/*
 * A170291  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40*(a(n-1)+…+a(n-40)) - 820*a(n-41) + p_n where p_n = 1
 *            for n==0 or n==41, 2 for 1≤n≤40, and 0 otherwise. This
 *            recurrence is derived from the generating function of A170291
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *acache = NULL;   /* cache array for a(0)…a(acap-1) */
static size_t asize = 0;       /* number of entries currently computed */
static size_t acap   = 0;      /* allocated capacity of the cache */

/* Ensure that a(0)…a(n) are computed and stored in acache. */
static void aensure(idx_t n)
{
    if ((size_t)n + 1 <= asize)  /* already computed */
        return;

    size_t need = (size_t)n + 1;        /* number of elements required */

    if (acap < need)                  /* enlarge the storage if necessary */
    {
        size_t newcap = acap ? acap << 1 : 16;
        while (newcap < need)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(acache, newcap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        acache = tmp;
        acap   = newcap;
    }

    /* Compute missing terms from asize up to n inclusive */
    for (idx_t i = (idx_t)asize; i <= n; ++i)
    {
        seq_t term = 0;

        /* a(0) is defined explicitly */
        if (i == 0)
        {
            acache[0] = 1;
            continue;
        }

        /* 40 * sum_{k=1}^{min(40,i)} a(i-k) */
        idx_t kmax = (i < 40) ? i : 40;
        for (idx_t k = 1; k <= kmax; ++k)
            term += acache[(size_t)(i - k)] * (seq_t)40;

        /* subtract 820 * a(i-41) when i >= 41 */
        if (i >= 41)
            term -= acache[(size_t)(i - 41)] * (seq_t)820;

        /* add the numerator coefficient p_i */
        seq_t p;
        if (i == 0 || i == 41)
            p = 1;
        else if (i <= 40)
            p = 2;
        else
            p = 0;

        term += p;

        acache[(size_t)i] = term;
    }

    asize = need;
}

seq_t a(idx_t n)
{
    aensure(n);
    return acache[(size_t)n];
}
