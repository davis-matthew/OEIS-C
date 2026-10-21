/*
 * A237042  UPC check digit for n.
 * https://oeis.org/A237042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = -( (Sum_{i=1..floor(L/2)} d(2i-1)) +
 *            3*(Sum_{j=0..floor(L/2)} d(2j)) ) mod 10 where L = number of
 *            decimal digits of n, d(0) = ones place, d(1) = tens, etc. This
 *            simply computes the UPC/EAN check digit
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    int pos = 0;                    /* position from the units: 0,1,2,...   */
    idx_t tmp = n;

    while (tmp != 0) {
        uint64_t digit = tmp % 10ULL;    /* current decimal digit           */
        seq_t weight = (pos & 1) ? 1 : 3;/* even pos (0,2,4,...) weight 3   */
        sum += digit * weight;          /* accumulate weighted sum          */
        tmp /= 10ULL;
        pos++;
    }

    /* a(n) = -sum mod 10  ==>  (10 - (sum % 10)) % 10  */
    seq_t rem = sum % 10;
    return (rem == 0) ? 0 : (10 - rem);
}
