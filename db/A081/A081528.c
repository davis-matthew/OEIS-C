/*
 * A081528  a(n) = n*lcm{1,2,...,n}.
 * https://oeis.org/A081528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * lcm{1,2,…,n}
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache of lcm values */
    static seq_t *lcms       = NULL;
    static size_t lcms_cap   = 0;
    static size_t lcms_size  = 0;

    if (n == 0)
        return 0;           /* offset is 1, n<1 is not used */

    /* Grow cache if needed */
    if (lcms_size < (size_t)n) {
        size_t need = (size_t)n;
        if (lcms_cap < need) {
            lcms_cap = need;
            seq_t *tmp = (seq_t *)realloc(lcms, lcms_cap * sizeof(seq_t));
            if (!tmp && lcms_cap > 0)
                return 0;   /* allocation failed */
            lcms = tmp;
        }

        /* Compute new lcm entries */
        for (size_t i = lcms_size; i < need; ++i) {
            if (i == 0) {
                lcms[0] = (seq_t)1;          /* lcm(1) = 1 */
            } else {
                seq_t prev = lcms[i - 1];
                seq_t cur  = (seq_t)(i + 1);  /* current integer i+1 */
                seq_t g    = gcd(prev, cur);
                lcms[i]    = (prev * cur) / g;
            }
        }
        lcms_size = need;
    }

    /* a(n) = n * lcm(1,…,n) */
    seq_t val = lcms[n - 1] * (seq_t)n;
    return val;
}
