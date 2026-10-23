/*
 * A179973  Number of permutations of [n] whose cycle lengths are
 *          nondecreasing when cycles are ordered by their minima and these
 *          minima are {1..k} (for some k <= n).
 * https://oeis.org/A179973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{lambda in partitions(n)} (n - |lambda|)!
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache */
    static idx_t  max_n = 0;
    static seq_t *fact = NULL;   /* factorials 0..max_n          */
    static seq_t *ans  = NULL;   /* a(0)..a(max_n)               */
    static seq_t *prev = NULL;   /* p(.,k-1) for current k      */
    static seq_t *curr = NULL;   /* p(.,k) for current k        */

    /* Recompute up to n if needed */
    if (ans == NULL || n > max_n)
    {
        /* Free old arrays if they exist */
        if (fact) free_seq_array(fact);
        if (ans)  free_seq_array(ans);
        if (prev) free_seq_array(prev);
        if (curr) free_seq_array(curr);

        size_t sz = (size_t)n + 1;
        fact = alloc_seq_array(sz);
        ans  = alloc_seq_array(sz);
        prev = alloc_seq_array(sz);
        curr = alloc_seq_array(sz);

        /* Factorials */
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            fact[i] = fact[i-1] * (seq_t)i;

        /* Initialise answer array */
        for (idx_t i = 0; i <= n; ++i)
            ans[i] = (seq_t)0;
        ans[0] = fact[0];   /* a(0) = 1 */

        /* Initialise partition array for k = 0 */
        for (idx_t i = 0; i <= n; ++i)
            prev[i] = (seq_t)0;
        prev[0] = (seq_t)1;

        /* Dynamic programming over partition parts k = 1..n */
        for (idx_t k = 1; k <= n; ++k)
        {
            /* Zero current array */
            for (idx_t i = 0; i <= n; ++i)
                curr[i] = (seq_t)0;

            /* Compute p(n,k) for all n >= k */
            for (idx_t m = k; m <= n; ++m)
                curr[m] = prev[m-1] + curr[m-k];

            /* Add (n-k)! * p(n,k) to the answer */
            for (idx_t m = k; m <= n; ++m)
                ans[m] += curr[m] * fact[m - k];

            /* Swap prev and curr for next k */
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }

        max_n = n;
    }

    return ans[n];
}
