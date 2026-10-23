/*
 * A127424  Numbers whose decimal expansion is a concatenation of 3
 *          consecutive decreasing numbers.
 * https://oeis.org/A127424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate(n+1, n, n-1)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = (n+1) * 10^(digits(n)+digits(n-1)) + n * 10^(digits(n-1)) + (n-1) */

#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t k1 = n + 1;   /* first number (k)            */
    idx_t k2 = n;       /* second number (k-1)         */
    idx_t k3 = n - 1;   /* third number (k-2)          */

    /* Number of decimal digits of the second and third numbers */
    int d2 = digits(k2);   /* digits(k)   */
    int d3 = digits(k3);   /* digits(k-1) */

    /* Compute the concatenation using powers of 10 */
    seq_t part1 = (seq_t)k1 * pow10((idx_t)(d2 + d3)); /* (k) * 10^(d2+d3)    */
    seq_t part2 = (seq_t)k2 * pow10((idx_t)d3);       /* (k-1) * 10^(d3)      */
    seq_t part3 = (seq_t)k3;                          /* (k-2)                */

    return part1 + part2 + part3;
}
