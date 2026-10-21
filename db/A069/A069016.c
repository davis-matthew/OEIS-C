/*
 * A069016  Look at all the different ways to factorize n as a product of
 *          numbers bigger than 1, and for each factorization write down the
 *          sum of the factors; a(n) = number of different sums.
 * https://oeis.org/A069016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3851, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A069016: number of distinct sums of all
   factorizations of n into integers greater than 1.
   a(1) = 1.  For n ≥ 2 the function enumerates every multiplicative
   partition of n, records the sum of each partition, and returns the
   number of distinct sums. */
#include <stdlib.h>
#include <stdint.h>

/* Recursive helper that processes factorizations of rem.
   rem   – the remaining part to factor
   start – the minimal factor that may be chosen next
   sum   – sum of the factors already chosen
   seen  – array with seen[sum] = 1 if that sum has been obtained
   n     – upper bound for indexing (size of seen) */
static void dfs(idx_t rem, idx_t start, seq_t sum, unsigned char *seen, idx_t n)
{
    for (idx_t f = start; f <= rem / f; ++f)
    {
        if (rem % f == 0)
        {
            dfs(rem / f, f, sum + (seq_t)f, seen, n);
        }
    }
    /* Record the factorization that ends with the current rem. */
    if (rem >= start)
    {
        seq_t tmp = sum + (seq_t)rem;
        idx_t  i   = (idx_t)tmp;     /* tmp ≤ n for all factorizations */
        seen[i] = 1;
    }
}

seq_t a(idx_t n)
{
    /* Special case: the sole factorization of 1 is (1). */
    if (n == 1u)
        return 1;

    /* Allocate flag array for possible sums 0 … n. */
    unsigned char *seen = (unsigned char *)calloc((size_t)n + 1, sizeof(unsigned char));
    if (!seen)           /* OOM unlikely for test limits; return 0 as failure */
        return 0;

    /* Enumerate all factorizations starting from the full number. */
    dfs(n, 2, 0, seen, n);

    /* Count distinct sums that actually appeared. */
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (seen[i]) ++count;

    free(seen);
    return count;
}
