/*
 * A187741  G.f.: Sum_{n>=0} (1 + n*x)^n * x^n / (1 + x + n*x^2)^n.
 * https://oeis.org/A187741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula for A187741:
 *   a(0)   = 1
 *   a(2*m)   = ((m+1)!)/2   (m >= 0)
 *   a(2*m-1) = m!          (m >= 1)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static factorial table up to a reasonable limit (max k ~ 5001 for n <= 10000) */
    /* fact[0] = 0! = 1 */
    static seq_t fact[10001] = {1};
    static idx_t fact_len = 1;  /* number of factorials currently computed (fact_len-1 is max k) */

    if (n == 0)
        return 1;

    if ((n & 1) == 0) { /* even n */
        idx_t m = n >> 1;           /* m = n/2          */
        idx_t k = m + 1;             /* k = m+1          */
        /* compute factorials up to k if needed */
        if (k >= fact_len) {
            for (idx_t i = fact_len; i <= k; ++i)
                fact[i] = fact[i - 1] * (seq_t)i;
            fact_len = k + 1;
        }
        return fact[k] / (seq_t)2;
    } else { /* odd n */
        idx_t k = (n + 1) >> 1; /* k = (n+1)/2     */
        /* compute factorials up to k if needed */
        if (k >= fact_len) {
            for (idx_t i = fact_len; i <= k; ++i)
                fact[i] = fact[i - 1] * (seq_t)i;
            fact_len = k + 1;
        }
        return fact[k];
    }
}
