/*
 * A062840  Palindromes of the form k^3 + 1.
 * https://oeis.org/A062840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
// a(2) = 1
// a(3) = 2
// a(4) = 9
// a(5) = 1001
// a(6) = 29792
// a(n) = 10^(3*(n-5)) + 1   for n >= 7
*/
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)9;
    if (n == 5) return (seq_t)1001;
    if (n == 6) return (seq_t)29792;

    /* n >= 7: a(n) = 10^(3*(n-5)) + 1 */
    idx_t exp = 3 * (n - 5);
    seq_t val = pow10(exp);
    return val + (seq_t)1;
}
