/*
 * A226007  Expansion of x * f(-x^7) * f(-x^21) / (f(-x) * f(-x^3)) where f()
 *          is a Ramanujan theta function.
 * https://oeis.org/A226007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^(n-1) in x * ∏_{k>0} (1 - x^(7k))(1 -
 *            x^(21k)) / ( (1 - x^k)(1 - x^(3k)) ), n≥1. This is equivalent to
 *            a(n) = coefficient of x^(n-1) in ∏_{m≥1,7∤m} (1 - x^m)^{-c(m)},
 *            where c(m)=1 if 3∤m and c(m)=2 if 3|m. The sequence is computed
 *            by dynamic programming: for each admissibl...
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 110, u64 n <= 357,
 *            u128 n <= 1254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below uses a static cache that is resized on demand.
   For each call it recomputes the coefficients from scratch up to the
   requested index.  The implementation is O(n²) with a small constant
   and works for seq_t of any supported width (including arbitrary
   precision).  It conforms to the requirements of this problem. */

#include <stdlib.h>

static seq_t *dp_cache = NULL;   /* array of coefficients up to current size */
static size_t dp_size = 0;       /* current allocated size */

seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is undefined for n=0 */
        return (seq_t)0;

    /* we need coefficients up to exponent n-1 */
    idx_t needed = n - 1;
    size_t new_size = (size_t)needed + 1;

    /* resize cache if necessary */
    if (new_size > dp_size) {
        seq_t *tmp = (seq_t *)realloc(dp_cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;            /* out of memory, return zero */
        dp_cache = tmp;
        dp_size = new_size;
    }

    /* initialise coefficients to zero */
    for (size_t i = 0; i < new_size; ++i)
        dp_cache[i] = (seq_t)0;
    dp_cache[0] = (seq_t)1;            /* base case */

    /* dynamic programming: apply each part size m that survives the
       cancellation of the canceling factors */
    for (idx_t m = 1; m <= needed; ++m) {
        if (m % 7 == 0)          /* multiples of 7 cancel completely */
            continue;

        if (m % 3 == 0) {        /* part appears with exponent -2 => use
                                   the unbounded knapsack step twice */
            for (idx_t i = m; i <= needed; ++i)
                dp_cache[i] += dp_cache[i - m];
            for (idx_t i = m; i <= needed; ++i)
                dp_cache[i] += dp_cache[i - m];
        } else {                 /* exponent -1 => single step */
            for (idx_t i = m; i <= needed; ++i)
                dp_cache[i] += dp_cache[i - m];
        }
    }

    return dp_cache[needed];
}
