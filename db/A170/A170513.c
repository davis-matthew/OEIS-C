/*
 * A170513  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170513: Number of reduced words of length n
   in the Coxeter group on 24 generators S_i with relations
   (S_i)^2 = (S_i S_j)^46 = I.
   The generating function is
   (1 + 2 t + 2 t^2 + … + 2 t^45 + t^46) /
   (1 - 22 t - 22 t^2 - … - 22 t^45 + 253 t^46).
   This yields the recurrence
       a(0) = 1;
   for 1 ≤ n ≤ 45:
       a(n) = 2 + 22 * (a(n-1) + … + a(0));
   for n = 46:
       a(46) = 1 + 22 * (a(45) + … + a(1)) - 253 * a(0);
   for n > 46:
       a(n) = 22 * (a(n-1) + … + a(n-45)) - 253 * a(n-46);
*/
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t  cache_len = 0;   /* number of elements currently stored */
static size_t  cache_cap = 0;   /* allocated capacity */

/* enlarge cache if necessary */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = 0;
        /* term 22 * sum of the previous 45 terms */
        for (idx_t k = 1; k <= 45 && k <= i; ++k)
            sum += cache[i - k];
        sum *= 22;
        /* subtract 253 * a_{i-46} when i >= 46 */
        if (i >= 46)
            sum -= cache[i - 46] * 253;

        /* constant term coming from the numerator of the G.f. */
        seq_t const_term = 0;
        if (i == 0)          const_term = 1;
        else if (i <= 45)    const_term = 2;
        else if (i == 46)    const_term = 1;

        cache[i] = sum + const_term;
    }
    cache_len = n + 1;
    return cache[n];
}
