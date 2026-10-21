/*
 * A099578  a(n) = binomial(floor((3n+2)/2), floor(n/2)).
 * https://oeis.org/A099578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(floor((3n+2)/2), floor(n/2))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute binomial coefficient C(n,k) for idx_t n,k.
   The routine keeps intermediate values small by cancelling
   factors of the denominator with the running result and with
   the current numerator before the final multiplication.  */
static seq_t my_binom(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i; /* current numerator term n-k+i   */
        seq_t den = i;         /* current denominator term i       */

        /* cancel common factors between result and den */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* cancel remaining factors between num and den */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* den should now be 1; multiply the reduced numerator */
        result = result * num;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t top = (3 * n + 2) / 2; /* floor((3n+2)/2) */
    idx_t bot = n / 2;           /* floor(n/2)     */

    return my_binom(top, bot);
}
