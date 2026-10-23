/*
 * A120788  Numerators of partial sums of Catalan numbers scaled by powers of
 *          -1/4.
 * https://oeis.org/A120788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(r(n)) where * r(n) = Sum_{k=0..n} (-1)^k * C(k)
 *            / 4^k, * C(k) = Catalan number (A000108)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r_num = (seq_t)1;           /* numerator of r(n)  */
    idx_t  r_exp = 0;                 /* denominator = 2^r_exp */

    seq_t catalan = (seq_t)1;         /* C(0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Sign of the current term: (-1)^k */
        int sign_neg = (k & 1) ? 1 : 0;

        /* Denominator for the new term: 4^k = 2^(2k) */
        idx_t e2 = 2 * k;

        if (r_exp < e2)   /* make common denominator 2^e2 */
        {
            idx_t shift = e2 - r_exp;
            seq_t shifted = r_num << shift;      /* r_num * 2^shift */

            if (sign_neg)
                shifted -= catalan;              /* subtract term   */
            else
                shifted += catalan;              /* add term        */

            r_num = shifted;
            r_exp = e2;
        }
        else               /* make common denominator 2^r_exp */
        {
            idx_t shift = r_exp - e2;
            seq_t shifted_c = catalan << shift; /* term  * 2^shift */

            if (sign_neg)
                r_num -= shifted_c;              /* subtract term   */
            else
                r_num += shifted_c;              /* add term        */
            /* r_exp unchanged */
        }

        /* Reduce by powers of two that divide the numerator */
        while (r_exp > 0 && ((r_num & (seq_t)1) == 0))
        {
            r_num >>= 1;
            --r_exp;
        }

        /* Compute next Catalan number C(k+1) using recurrence:
         * C_{k+1} = C_k * 2*(2*k+1) / (k+2)   */
        seq_t mult = catalan * ( (seq_t)2 * ((seq_t)2 * k + (seq_t)1) );
        catalan = mult / ((seq_t)k + 2);
    }

    /* For n == 0, loop is skipped and r_num == 1 (already correct) */
    return r_num;
}
