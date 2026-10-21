/*
 * A225346  Number of 6Xn -1,1 arrays such that the sum over i=1..6,j=1..n of
 *          i*x(i,j) is zero, the sum of x(i,j) is zero, and rows are
 *          nondecreasing (number of ways to distribute n-across galley
 *          oarsmen left-right at 6 fore-aft positions so that there are no
 *          turning moments on the ship).
 * https://oeis.org/A225346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A225346: Number of 6×n arrays of -1,1 with sum over all entries
 *            = 0, weighted sum ∑ i*x_{i,j} = 0, rows nondecreasing. The
 *            sequence satisfies the empirical recurrence a(n) =
 *            a(n-2)+2*a(n-4)-a(n-6)-2*a(n-8)+a(n-12)-a(n-14)+2*a(n-18)+a(n-20)
 *            -2*a(n-22)-a(n-24)+a(n-26) with initial terms a(1)=0, a(2...
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   count_even_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_even_n.h"

/* Forward declaration – seq_t and idx_t are provided by the build system. */
extern seq_t a(idx_t n);

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index 0 is outside the offset */
    if (n % 2) return 0;           /* odd n → zero entries */

    /* Static cache to store computed values up to the largest n requested */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated capacity (indices 0..cap-1) */
    static idx_t computed = 0;     /* up to but not including this index filled */

    /* Ensure capacity */
    if (n + 1 > cap)
    {
        idx_t new_cap = (n + 1) * 2;   /* grow twice to limit reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure → zero */
        cache = tmp;
        /* initialise the new part to zero */
        for (idx_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Fill in missing values up to n */
    if (computed <= n)
    {
        for (idx_t i = computed; i <= n; ++i)
        {
            if (i <= 26)
            {
                /* small even values – compute directly */
                if (i % 2)
                    cache[i] = 0;
                else
                    cache[i] = count_even_n(i);
            }
            else
            {
                /* Empirical recurrence for n ≥ 28 (holds for all even n > 26) */
                seq_t sum  = cache[i-2];
                sum += cache[i-4];
                sum += cache[i-4];          /* ×2 */
                sum += cache[i-12];
                sum += cache[i-18];
                sum += cache[i-18];         /* ×2 */
                sum += cache[i-20];
                sum += cache[i-26];

                seq_t diff = cache[i-6];
                diff += cache[i-8];
                diff += cache[i-8];          /* ×2 */
                diff += cache[i-14];
                diff += cache[i-22];
                diff += cache[i-22];         /* ×2 */
                diff += cache[i-24];

                cache[i] = sum - diff;
            }
        }
        computed = n + 1;
    }
    return cache[n];
}
