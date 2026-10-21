/*
 * A062987  Row sums of unsigned N(5) staircase array A062986.
 * https://oeis.org/A062987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 1, a(1) = 31
 *   For n ≥ 2:
 *     a(n) = ( (6218*n⁴ - 12484*n³ + 8752*n² - 2501*n + 240) * a(n-1)
 *            + 10*(5*n-4)*(5*n-3)*(5*n-2)*(5*n-1) * a(n-2) )
 *            / ( n*(2*n-1)*(4*n-1)*(4*n+1) )
 *   (All operations are exact integer divisions.)
 *
 *   This recurrence is taken from OEIS A062987.
 */
#include <stdlib.h>

/* Memoization array */
static seq_t *memo = NULL;
static idx_t memo_size = 0;  /* highest index in memo + 1 */
static idx_t memo_cap  = 0;  /* allocated size */

/* Grow memo array to accommodate indices up to <= need */
static void grow_memo(idx_t need)
{
    if (need < memo_cap)
        return;
    idx_t new_cap = memo_cap ? memo_cap * 2 : 2;
    while (new_cap <= need)
        new_cap *= 2;
    seq_t *p = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!p)
        exit(1);
    memo = p;
    memo_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)31;

    /* Ensure memo array can hold index n */
    grow_memo(n);

    /* Fill memo from current size up to n */
    for (idx_t i = memo_size; i <= n; ++i)
    {
        if (i == 0) {
            memo[i] = (seq_t)1;
        } else if (i == 1) {
            memo[i] = (seq_t)31;
        } else {
            /* All intermediate arithmetic performed with unsigned __int128
             * to avoid overflow for 64‑bit seq_t builds.
             */
#ifdef __SIZEOF_INT128__
            unsigned __int128 ii = i;
            unsigned __int128 ip2 = ii * ii;
            unsigned __int128 ip3 = ip2 * ii;
            unsigned __int128 ip4 = ip3 * ii;

            unsigned __int128 term1 = (unsigned __int128)6218 * ip4
                                    - (unsigned __int128)12484 * ip3
                                    + (unsigned __int128)8752 * ip2
                                    - (unsigned __int128)2501 * ii
                                    + (unsigned __int128)240;

            unsigned __int128 t1 = (unsigned __int128)(5*i - 4);
            unsigned __int128 t2 = (unsigned __int128)(5*i - 3);
            unsigned __int128 t3 = (unsigned __int128)(5*i - 2);
            unsigned __int128 t4 = (unsigned __int128)(5*i - 1);

            unsigned __int128 term2 = (unsigned __int128)10 * t1 * t2 * t3 * t4;

            unsigned __int128 num1 = term1 * (unsigned __int128)memo[i-1];
            unsigned __int128 num2 = term2 * (unsigned __int128)memo[i-2];

            unsigned __int128 numerator   = num1 + num2;
            unsigned __int128 denom       = ii
                                            * (unsigned __int128)(2*i - 1)
                                            * (unsigned __int128)(4*i - 1)
                                            * (unsigned __int128)(4*i + 1);

            memo[i] = (seq_t)(numerator / denom);
#else
            /* Fallback to 64‑bit arithmetic when __int128 is unavailable.
             * This path is sufficient for seq_t ≥ 16 bits only for very
             * small n because intermediate products may overflow.
             */
            idx_t ii = i;
            idx_t ip2 = ii * ii;
            idx_t ip3 = ip2 * ii;
            idx_t ip4 = ip3 * ii;

            unsigned long long term1 = (unsigned long long)6218 * ip4
                                     - (unsigned long long)12484 * ip3
                                     + (unsigned long long)8752 * ip2
                                     - (unsigned long long)2501 * ii
                                     + (unsigned long long)240;

            unsigned long long t1 = (unsigned long long)(5*i - 4);
            unsigned long long t2 = (unsigned long long)(5*i - 3);
            unsigned long long t3 = (unsigned long long)(5*i - 2);
            unsigned long long t4 = (unsigned long long)(5*i - 1);

            unsigned long long term2 = (unsigned long long)10 * t1 * t2 * t3 * t4;

            unsigned long long num1 = term1 * (unsigned long long)memo[i-1];
            unsigned long long num2 = term2 * (unsigned long long)memo[i-2];

            unsigned long long numerator = num1 + num2;
            unsigned long long denom = ii * (unsigned long long)(2*i - 1)
                                      * (unsigned long long)(4*i - 1)
                                      * (unsigned long long)(4*i + 1);

            memo[i] = (seq_t)(numerator / denom);
#endif
        }
    }
    memo_size = n + 1;
    return memo[n];
}
