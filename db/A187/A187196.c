/*
 * A187196  McKay-Thompson series of class 12E for the Monster group with a(0)
 *          = -2.
 * https://oeis.org/A187196
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^n in 1/q *
 *            (eta(q)*eta(q^3)/(eta(q^4)*eta(q^12)))^2
 * Range:     i8 n <= 18, i16 n <= 60, i32 n <= 208, i64 n <= 738,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The function returns the nth term of sequence A187196
   for n >= -1.  Each call may request any n, and the
   previously computed range is reused.                         */
seq_t a(idx_t n)
{
    if (n < -1)            /* behaviour below the offset is irrelevant */
        return 0;

    /* We need the coefficient of q^(n+1) in the
       product (eta(q)*eta(q^3)/(eta(q^4)*eta(q^12)))^2   */
    size_t req = (size_t)(n + 1);

    /* Static storage for computed coefficients */
    static seq_t *coeff = NULL;
    static size_t  len   = 0;   /* current allocated length */

    if (req >= len)    /* enlarge and recompute up to the new maximum */
    {
        /* Allocate new storage */
        seq_t *newcoeff = (seq_t*)malloc((req + 1) * sizeof(seq_t));
        if (!newcoeff) exit(1);

        /* initialise coefficients to zero */
        for (size_t i = 0; i <= req; ++i) newcoeff[i] = 0;
        newcoeff[0] = 1;     /* constant term of the product */

        /* Build the product (1 - q^k)^e_k for k = 1 .. req
           where e_k = 0 if k % 4 == 0,
                           4 if k % 3 == 0 and k % 4 != 0,
                           2 otherwise.                      */
        for (size_t k = 1; k <= req; ++k)
        {
            int e;
            if (k % 4 == 0)          e = 0;
            else if (k % 3 == 0)     e = 4;
            else                     e = 2;

            /* Equivalent to multiplying by (1 - q^k)^e
               using e successive passes of (1 - q^k)                     */
            for (int t = 0; t < e; ++t)
            {
                for (size_t i = req; ; --i)
                {
                    newcoeff[i] -= newcoeff[i - k];
                    if (i == k) break;
                }
            }
        }

        /* Replace old array */
        free(coeff);
        coeff = newcoeff;
        len   = req + 1;
    }

    return coeff[req];   /* a(n) = coefficient of q^(n+1)   */
}
