/*
 * A185986  1/128 the number of (n+2)X4 binary arrays with each 3X3 subblock
 *          trace equal to some horizontal or vertical neighbor 3X3 subblock
 *          trace.
 * https://oeis.org/A185986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * * a(n) satisfies the linear recurrence * * a(n) = 10*a(n-1) -
 *            11*a(n-2) - 6*a(n-3) + 51*a(n-4) + 30*a(n-5) * - 330*a(n-6) -
 *            190*a(n-7) + 568*a(n-8) - 1888*a(n-9) * + 3442*a(n-10) -
 *            2512*a(n-11) + 328*a(n-12) - 1862*a(n-13) * + 4596*a(n-14) -
 *            1224*a(n-15) - 432*a(n-16) * * with initial terms * *...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for NULL */

seq_t a(idx_t n)
{
    /* Initial twelve terms, stored as 64‑bit literals (they fit into
     * any unsigned integer type used by the build).                 */
    static const seq_t init[16] = {
        10ULL,                    /* a(1)   */
        76ULL,                    /* a(2)   */
        722ULL,                   /* a(3)   */
        6036ULL,                  /* a(4)   */
        52966ULL,                 /* a(5)   */
        462288ULL,                /* a(6)   */
        4038360ULL,               /* a(7)   */
        35286678ULL,              /* a(8)   */
        308300326ULL,             /* a(9)   */
        2693690508ULL,            /* a(10)  */
        23535373610ULL,           /* a(11)  */
        205633797210ULL,          /* a(12)  */
        1796669120244ULL,         /* a(13)  */
        15697904088278ULL,        /* a(14)  */
        137156137242726ULL,       /* a(15)  */
        1198364180196068ULL       /* a(16)  */
    };

    /* Recurrence coefficients ------------------------------------------------- */
    static const long long coeff[16] = {
        10,   -11,  -6,   51,   30,  -330, -190,  568,
        -1888,3442, -2512, 328, -1862, 4596, -1224, -432
    };

    /* Return a pre‑computed term if the index is small.                         */
    if (n <= 16)
        return init[n - 1];

    /* Sliding window holding the last 16 terms.                                 */
    seq_t buf[16];
    for (int i = 0; i < 16; ++i)
        buf[i] = init[i];

    /* Compute successive terms a(17) … a(n) --------------------------------- */
    for (idx_t k = 16; k < n; ++k)
    {
        seq_t pos = 0;          /* sum of positive contributions                 */
        seq_t neg = 0;          /* sum of negative contributions (absolute value) */

        for (int i = 0; i < 16; ++i)
        {
            /* absolute value of the coefficient, converted to seq_t          */
            seq_t factor =
                (seq_t)(coeff[i] >= 0 ? coeff[i] : -coeff[i]);

            /* product of the stored term and the absolute coefficient      */
            seq_t prod = buf[(k - 1 - i) & 15] * factor;

            if (coeff[i] >= 0)
                pos += prod;
            else
                neg += prod;
        }

        /* Final term is the difference of the two sums (positive minus
         * negative).  For unsigned seq_t this is a correct subtraction
         * because the result is guaranteed to be non‑negative.           */
        buf[k & 15] = pos - neg;
    }

    /* Return the term at position n.                                          */
    return buf[(n - 1) & 15];
}
