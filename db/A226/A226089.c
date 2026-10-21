/*
 * A226089  Denominators of the series b(n+1) = (b(n)+k)/(1+b(n)*k); where k =
 *          1/(n+1), b(1) = 0.
 * https://oeis.org/A226089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1. For n > 1 let b_{n-1} = p/q be the reduced fraction
 *            representing the relativistic sum up to 1/(n). Then new
 *            numerator = p·n + q new denominator = q·n + p The fraction is
 *            reduced by dividing numerator and denominator by their gcd. a(n)
 *            is the resulting denominator
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 3065, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* b(1) = 0/1, denominator 1. */
    seq_t p = 0;          /* numerator of b(n)   */
    seq_t q = 1;          /* denominator of b(n) */

    if (n <= 1)            /* a(1) = 1 */
        return q;

    for (idx_t i = 1; i < n; ++i)   /* compute b(i+1) for i = 1 .. n-1 */
    {
        /* k = 1/(i+1) -> use n = i+1 for the recurrence */
        seq_t step = (seq_t)(i + 1);

        seq_t new_p = p * step + q;   /* numerator before reduction   */
        seq_t new_q = q * step + p;   /* denominator before reduction */

        seq_t g = gcd(new_p, new_q);   /* reduce fraction            */
        p = new_p / g;
        q = new_q / g;
    }
    return q;   /* denominator of b(n) */
}
