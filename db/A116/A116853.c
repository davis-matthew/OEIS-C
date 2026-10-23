/*
 * A116853  Difference triangle of factorial numbers read by upward diagonals.
 * https://oeis.org/A116853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{i=0}^{k} (-1)^{k-i} * binom(k, i) * (j + i + 1)! where
 *            r = smallest integer with r(r+1)/2 ≥ n (row number) j = n -
 *            (r-1)r/2 - 1 (column index starting from 0) k = r-1-j
 *            (difference order)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 86, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Forward declarations for seq_t handling */
static void ensure_fact(idx_t n);

/* Cached factorials: fact[i] = i!  (0! = 1) */
static seq_t *fact = NULL;
static size_t fact_size = 0;   /* maximum index currently stored */

/* Ensure factorials up to n (inclusive) are precomputed */
static void
ensure_fact(idx_t n)
{
    if ((size_t)n <= fact_size)
        return;

    size_t new_size = (size_t)n;
    seq_t *new_fact = (seq_t *)realloc(fact, (new_size + 1) * sizeof(seq_t));
    if (!new_fact)
        exit(1);   /* allocation failed */

    fact = new_fact;

    /* if this is the first allocation, set 0! = 1 */
    if (fact_size == 0)
        fact[0] = (seq_t)1;

    seq_t f = fact[fact_size];
    for (idx_t i = fact_size + 1; i <= new_size; ++i)
    {
        f = f * (seq_t)i;
        fact[i] = f;
    }
    fact_size = new_size;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined below the offset */

    /* Find r such that r(r+1)/2 >= n   (row number) */
    idx_t lo = 1, hi = 1;
    while (1)
    {
        __int128 tmp = (__int128)hi * (hi + 1) / 2;
        if (tmp >= n)
            break;
        hi <<= 1;   /* double hi */
    }
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 tmp = (__int128)mid * (mid + 1) / 2;
        if (tmp >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* compute j and k */
    __int128 t_prev = (__int128)(r - 1) * r / 2;   /* triangular number T_{r-1} */
    idx_t j = n - (idx_t)t_prev - 1;   /* column index */
    idx_t k = r - 1 - j;               /* difference order */

    /* ensure factorials up to r are available */
    if ((idx_t)r > fact_size)
        ensure_fact(r);

    /* evaluate the sum */
    seq_t pos = 0, neg = 0;
    seq_t c = 1;   /* binomial coefficient C(k,0) */

    for (idx_t i = 0; i <= k; ++i)
    {
        seq_t term = fact[j + i + 1];   /* (j + i + 1)! */
        term *= c;                      /* multiply by binomial coefficient */

        if (((k - i) & 1) == 0)   /* sign + if (k - i) even */
            pos += term;
        else
            neg += term;

        /* update binomial coefficient for next i */
        if (i < k)
        {
            c *= (k - i);
            c /= (i + 1);
        }
    }

    /* pos >= neg for all valid indices */
    return pos - neg;
}
