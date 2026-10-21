/*
 * A082941  a(n) is the odd-length palindrome whose digits up to the center
 *          are those of n and whose center digit is equal to the digital root
 *          of the product of the factorial of n and the reverse of n.
 * https://oeis.org/A082941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = integer with digits of n, a center digit d(n), and
 *            reverse of n as suffix. Center digit: d(1)=1, d(2)=4, d(n)=9 for
 *            n>=3
 * Range:     u8 n <= 2, u16 n <= 64, u32 n <= 4999, u64 n <= 4999,
 *            u128 n <= 4999, big any n
 * Helpers:   digits_of, pow10, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/digits_of.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute the number of decimal digits of n */
    int nd = digits_of(n);                  /* nd >= 1 for all n >= 1 */

    /* reverse of n without leading zeros */
    seq_t rev_n = (seq_t)rev(n);

    /* powers of 10 required for shifting */
    seq_t pow1 = pow10((idx_t)(nd + 1));     /* 10^(nd+1) */
    seq_t pow2 = pow10((idx_t)nd);          /* 10^nd            */

    /* determine center digit */
    seq_t center;
    if (n == 1)
        center = 1;                         /* digital root 1! * rev(1!) = 1 */
    else if (n == 2)
        center = 4;                         /* digital root 2! * rev(2!) = 4 */
    else
        center = 9;                         /* digital root >2 always 9 */

    /* assemble the palindrome: left part | center | right part */
    seq_t result = ((seq_t)n) * pow1 + center * pow2 + rev_n;
    return result;
}
