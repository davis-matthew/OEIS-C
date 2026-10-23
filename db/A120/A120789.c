/*
 * A120789  Numerators of partial sums of Catalan numbers scaled by powers of
 *          -1/8.
 * https://oeis.org/A120789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: a(n) = numerator of the reduced fraction of
   r(n) = sum_{k=0..n} ((-1)^k)*Catalan(k)/8^k.
   The denominator of r(n) is always a power of 2; the
   algorithm keeps that power as an exponent.
   For each k we add the term (sign * C(k)) / 2^(3k) to
   the current rational number p / 2^e and then cancel
   all common factors of 2.  Catalan numbers are computed
   with the recurrence
          C_0 = 1,
          C_k = C_{k-1} * (4k-2) / (k+1)   (k >= 1).
   All arithmetic is done with seq_t; the exponent is kept
   in a signed 64‑bit integer.
*/

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Initial value for r(0) = 1 */
    seq_t num = (seq_t)1;            // numerator (odd)
    long long exp_den = 0;           // denominator exponent (2^exp_den)
    seq_t cat = (seq_t)1;            // Catalan(0)

    idx_t k;
    for (k = 1; k <= n; ++k)
    {
        /* compute Catalan(k) from Catalan(k-1) */
        cat = cat * (seq_t)(4 * k - 2) / (seq_t)(k + 1);

        bool negative = (k & 1);                /* sign = (-1)^k */

        idx_t denom_k = 3 * k;                  /* exponent of 2 in 8^k */

        long long m = exp_den > (long long)denom_k ? exp_den : (long long)denom_k;

        int shift_p = (int)(m - exp_den);      /* shift for previous numerator   */
        int shift_t = (int)(m - denom_k);      /* shift for current term          */

        seq_t shift_p_val = num << shift_p;
        seq_t shift_t_val = cat << shift_t;

        seq_t new_num = shift_p_val;
        if (negative)
            new_num -= shift_t_val;
        else
            new_num += shift_t_val;

        long long new_exp = m;

        /* cancel all factors of 2 */
        while ((new_num & 1) == (seq_t)0)
        {
            new_num >>= 1;
            new_exp--;
        }

        num = new_num;
        exp_den = new_exp;
    }

    return num;   /* numerator of the reduced r(n) */
}
