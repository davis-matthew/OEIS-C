/*
 * A163030  Number of n X 4 binary arrays with all 1s connected and a path of
 *          1s from top row to bottom row.
 * https://oeis.org/A163030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) -59*a(n-2) +97*a(n-3) -19*a(n-4) -210*a(n-5)
 *            +222*a(n-6) +22*a(n-7) -113*a(n-8) +7*a(n-9) -71*a(n-10)
 *            +13*a(n-11) +a(n-12) (offset = 1: a(1)=10, a(2)=88, …,
 *            a(12)=978984263266)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* no defined value for n < 1 */

    /* Cache up to 10000 entries (sufficient for the test limits). */
    static seq_t cache[10001];
    static size_t max_computed = 0;   /* number of entries actually computed (0‑based) */

    /* Initial 12 terms (1‑based). */
    static const unsigned long long init_vals[12] = {
        10ULL, 88ULL, 920ULL, 9362ULL, 94280ULL,
        947760ULL, 9528128ULL, 95797726ULL, 963186740ULL,
        9684246062ULL, 97369020768ULL, 978984263266ULL
    };

    /* Make sure we have computed terms up to n-1 (0‑based). */
    if ((size_t)n > max_computed)
    {
        /* Fill the initial block if we have not already. */
        size_t i;
        for (i = 0; i < 12 && i < (size_t)n; ++i)
            cache[i] = (seq_t)init_vals[i];

        /* Update the highest index we are certain about. */
        if (i > max_computed) max_computed = i;

        /* Compute remaining terms using the recurrence. */
        for (i = max_computed; i < (size_t)n; ++i)
        {
            /* Positive contributions: 15, 97, 222, 22, 7, 13, 1 */
            seq_t pos = (seq_t)15   * cache[i-1]
                      + (seq_t)97   * cache[i-3]
                      + (seq_t)222  * cache[i-6]
                      + (seq_t)22   * cache[i-7]
                      + (seq_t)7    * cache[i-9]
                      + (seq_t)13   * cache[i-11]
                      +            cache[i-12];

            /* Negative contributions: 59, 19, 210, 113, 71 */
            seq_t neg = (seq_t)59   * cache[i-2]
                      + (seq_t)19   * cache[i-4]
                      + (seq_t)210  * cache[i-5]
                      + (seq_t)113  * cache[i-8]
                      + (seq_t)71   * cache[i-10];

            cache[i] = pos - neg;   /* pos >= neg for all n */
        }

        /* Mark all terms up to n-1 as computed. */
        max_computed = n;
    }

    return cache[n-1];  /* convert from 0‑based index to 1‑based */
}
