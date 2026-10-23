/*
 * A011569  Stirling numbers of second kind S2(20,n).
 * https://oeis.org/A011569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling number of the second kind of 20 and n, S(20,n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The function returns S(20,n) for 1 <= n <= 20 and 0 otherwise. */

seq_t a(idx_t n)
{
    /* Static cache for S(20,k) values, computed once */
    static seq_t cache[21];
    static bool computed = false;

    if (!computed)
    {
        /* Dynamic programming with two rows for S(i,k) */
        seq_t prev[21] = {0};
        seq_t curr[21] = {0};

        prev[0] = 1;   /* S(0,0) = 1 */

        for (idx_t i = 1; i <= 20; ++i)
        {
            curr[0] = 0;   /* S(i,0) = 0 for i > 0 */

            for (idx_t k = 1; k <= i; ++k)
            {
                /* S(i,k) = k*S(i-1,k) + S(i-1,k-1) */
                seq_t term1 = (seq_t)k * prev[k];
                seq_t term2 = prev[k-1];
                curr[k] = term1 + term2;
            }

            /* Copy current row to prev for next iteration */
            for (idx_t k = 0; k <= i; ++k)
                prev[k] = curr[k];
        }

        /* Store the final row (i = 20) into the cache */
        for (idx_t k = 1; k <= 20; ++k)
            cache[k] = prev[k];
        cache[0] = 0;  /* unused */
        computed = true;
    }

    if (n >= 1 && n <= 20)
        return cache[(idx_t)n];
    else
        return (seq_t)0;   /* S(20,n) = 0 for n > 20 or n < 1 */
}
