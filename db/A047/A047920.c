/*
 * A047920  Triangular array formed from successive differences of factorial
 *          numbers.
 * https://oeis.org/A047920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = T(r,k), where r is the row index satisfying r*(r+1)/2 <=
 *            n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2. T(r,k) = Σ_{j=0..k}
 *            (-1)^j * binom(k,j) * (r-j)!
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th element. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;   /* column within the row */

    /* --- factorial cache ------------------------------------------------- */
    static seq_t *fact_cache = NULL;
    static idx_t fact_size   = 0;

    if (fact_size <= r)
    {
        size_t new_size = (size_t)(r + 1);
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* out of memory – unlikely */
        fact_cache = tmp;

        if (fact_size == 0)               /* initialise 0! */
            fact_cache[0] = (seq_t)1;

        for (idx_t i = fact_size; i <= r; ++i)
        {
            if (i == 0) continue;          /* 0! already set */
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_size = new_size;
    }

    /* --- binomial coefficient cache for the current k ------------------- */
    static seq_t *bin_cache = NULL;
    static idx_t bin_size   = 0;

    if (bin_size <= k)
    {
        size_t new_size = (size_t)(k + 1);
        seq_t *tmp = (seq_t *)realloc(bin_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        bin_cache = tmp;
        bin_size  = new_size;
    }

    bin_cache[0] = (seq_t)1;
    for (idx_t j = 1; j <= k; ++j)
        /* C(k,j) = C(k,j-1) * (k-j+1) / j */
        bin_cache[j] = (bin_cache[j - 1] * (seq_t)(k - j + 1)) / (seq_t)j;

    /* --- compute T(r,k) ------------------------------------------------- */
    seq_t result = (seq_t)0;
    for (idx_t j = 0; j <= k; ++j)
    {
        seq_t term = bin_cache[j] * fact_cache[r - j];
        if (j & 1)
            result -= term;
        else
            result += term;
    }

    return result;
}
