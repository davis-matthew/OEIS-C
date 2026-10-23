/*
 * A167124  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15·Σ_{i=1}^{13} a(n−i) – 120·a(n−14) + b(n) * where *
 *            b(0)=1, b(1)…b(13)=2, b(14)=1, b(n)=0 for n>14 * a(k)=0 for k<0.
 *            * Generates the sequence A167124
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* numerator coefficient b(n) */
static inline seq_t b_term(idx_t n)
{
    if (n == 0U)          return (seq_t)1;
    if (n <= 13U)         return (seq_t)2;
    if (n == 14U)         return (seq_t)1;
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    /* memoised cache */
    static seq_t *cache      = NULL;  /* array of computed terms           */
    static idx_t max_alloc   = 0U;    /* currently allocated capacity       */
    static idx_t computed_len = 0U;   /* number of terms already computed  */

    /* allocate / expand cache if needed */
    if (n + 1U > max_alloc)
    {
        idx_t new_len = n + 1U;
        if (max_alloc == 0U)
            cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        else
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        max_alloc = new_len;
    }

    /* compute all missing terms up to n */
    for (idx_t k = computed_len; k <= n; ++k)
    {
        seq_t sum = 0;

        /* sum of the previous 13 terms, if they exist */
        for (idx_t i = 1U; i <= 13U; ++i)
        {
            if (k >= i)
                sum += cache[k - i];
        }

        sum *= (seq_t)15;   /* multiply by 15 */

        if (k >= 14U)
            sum -= cache[k - 14] * (seq_t)120;  /* subtract 120·a(k-14) */

        sum += b_term(k);    /* add numerator coefficient */

        cache[k] = sum;
    }

    computed_len = n + 1U;   /* all terms up to n are now stored */

    return cache[n];
}
