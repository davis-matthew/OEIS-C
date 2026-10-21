/*
 * A081479  Consider the mapping f(a/b) = (a^2 +b^2)/(a+b). Taking a =1, b = 2
 *          to start with and carrying out this mapping repeatedly on each new
 *          (reduced) rational number gives the following sequence
 *          1/2,5/3,17/4,305/21,... Sequence contains the numerators.
 * https://oeis.org/A081479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081479
   a(1) = 1
   a(n+1) = numerator of  f(a(n)/b(n)) = (a(n)^2 + b(n)^2)/(a(n)+b(n))
   after reducing (a(n+1), b(n+1)) by gcd.
   The sequence terms listed are the numerators of the successive fractions.
   Example: 1/2 → 5/3 → 17/4 …
*/

#include <stdbool.h>   /* for bool if needed */

/* Euclidean algorithm for seq_t */
static seq_t seq_gcd(seq_t x, seq_t y)
{
    while (y)
    {
        seq_t r = x % y;
        x = y;
        y = r;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* base case: first term is 1/2 → numerator 1 */
    if (n <= 1)
        return (seq_t)1;

    /* start with 1/2 */
    seq_t num = (seq_t)1;
    seq_t den = (seq_t)2;

    /* iterate until nth term */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t new_num = num * num + den * den;
        seq_t new_den = num + den;
        seq_t g = seq_gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;
        num = new_num;
        den = new_den;
    }
    return num;
}
