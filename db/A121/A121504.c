/*
 * A121504  Numerators of partial sums of a series used for the series of
 *          sqrt(2) + sqrt(3) involving Catalan numbers.
 * https://oeis.org/A121504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator(r(n)) where r(n) = Σ_{k=0..n}
 *            C(k)*(1+2^(k+1))/16^k (C(k) = Catalan numbers)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Cached data for memoisation. */
    static seq_t *num = NULL;       /* numerators r(k) */
    static idx_t *den = NULL;       /* exponents of 2 in the denominators */
    static size_t cap = 0;          /* allocated size */
    static idx_t curr = 0;          /* highest index computed so far */

    /* Auxiliaries that are kept in sync with k. */
    static seq_t cat = 1;           /* Catalan(k) for current k */
    static seq_t pow2kp1 = 2;       /* 2^(k+1) for current k */

    /* Initialise for k = 0. */
    if (num == NULL)
    {
        num  = (seq_t *)malloc(sizeof(seq_t));
        den  = (idx_t *)malloc(sizeof(idx_t));
        if (num == NULL || den == NULL)
            return (seq_t)0;                 /* allocation failure */
        num[0] = (seq_t)3;                  /* r(0) = 3 */
        den[0] = 0;                          /* denominator 1 = 2^0 */
        cap    = 1;
        curr   = 0;
    }

    /* Resize the arrays if necessary. */
    if ((size_t)(n + 1) > cap)
    {
        seq_t *t_num = (seq_t *)realloc(num, (size_t)(n + 1) * sizeof(seq_t));
        idx_t *t_den = (idx_t *)realloc(den, (size_t)(n + 1) * sizeof(idx_t));
        if (t_num == NULL || t_den == NULL)
            return (seq_t)0;                     /* allocation failure */
        num = t_num;
        den = t_den;
        cap = (size_t)(n + 1);
    }

    /* Compute all terms up to n. */
    for (idx_t k = curr + 1; k <= n; ++k)
    {
        /* Update Catalan(k).  C(k) = C(k-1) * 2*(2k-1)/(k+1). */
        cat = cat * (seq_t)(2 * (2 * k - 1)) / (seq_t)(k + 1);

        /* 2^(k+1) for this k. */
        pow2kp1 <<= 1;                    /* shift left once */

        /* Term numerator and its exponent. */
        seq_t term_num  = cat * (pow2kp1 + (seq_t)1); /* C(k)*(1+2^(k+1)) */
        idx_t  term_exp = 4 * k;          /* since 16^k = 2^(4k) */

        /* Previous sum: prev_num / 2^prev_exp. */
        seq_t prev_num = num[k - 1];
        idx_t  prev_exp = den[k - 1];

        /* Align denominators. */
        idx_t new_exp = prev_exp > term_exp ? prev_exp : term_exp;
        seq_t new_num = (prev_num << (new_exp - prev_exp))
                      + (term_num  << (new_exp - term_exp));

        /* Cancel powers of 2. */
        while ((new_num & (seq_t)1) == 0)
        {
            new_num >>= 1;
            new_exp--;
        }

        /* Store the result. */
        num[k] = new_num;
        den[k] = new_exp;
    }

    curr = n;
    return num[n];
}
