/*
 * A127888  If X_1,...,X_n is a partition of a 6n-set X into 6-blocks then
 *          a(n) is equal to the number of permutations f of X such that
 *          f(X_i)<>X_i, (i=1,...n).
 * https://oeis.org/A127888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ((-720)^i * binomial(n,i) * (6*n-6*i)!, i = 0..n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   binomial, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

static seq_t *fact6_cache = NULL;   /* fact6[k] = (6*k)!              */
static idx_t fact6_cap   = 0;       /* current maximum k in cache     */

/* Ensure the factorial table contains entries up to k = n. */
static void ensure_fact6(idx_t n)
{
    if (n <= fact6_cap)
        return;

    /* Grow the cache to size n+1 */
    fact6_cache = (seq_t *)realloc(fact6_cache, (n + 1) * sizeof(seq_t));
    if (!fact6_cache)
        abort();  /* simple error handling */

    /* Compute new factorial entries */
    for (idx_t k = fact6_cap; k <= n; ++k) {
        if (k == 0) {
            fact6_cache[0] = 1;
            continue;
        }
        seq_t val = fact6_cache[k - 1];
        /* Multiply by the numbers (6*(k-1)+1) .. (6*k) */
        idx_t start = 6 * (k - 1) + 1;
        idx_t end   = 6 * k;
        for (idx_t mul = start; mul <= end; ++mul)
            val = mul_small(val, (unsigned)mul);
        fact6_cache[k] = val;
    }
    fact6_cap = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, but harmless here */
        return 0;

    ensure_fact6(n);     /* precompute (6*k)! up to k = n */

    seq_t result = 0;
    seq_t power  = 1;    /* 720^i as i varies from 0 to n */

    for (idx_t i = 0; i <= n; ++i) {
        /* term = C(n,i) * (6n-6i)! * (-720)^i */
        seq_t term = fact6_cache[n - i];          /* (6(n-i))!  */

        term = seq_mul(term, power);             /* × 720^i   */
        term = seq_mul(term, binomial(n, i));    /* × C(n,i)  */

        if (i & 1)                /* odd i → negative sign */
            result = result - term;
        else
            result = result + term;

        /* update 720^i → 720^(i+1) */
        power = mul_small(power, 720);
    }

    return result;
}
