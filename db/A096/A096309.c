/*
 * A096309  a(1)=1; for n > 1, a(n) is the number of levels in the "stacked"
 *          prime number factorization of n (prime number factorization of the
 *          exponents if necessary and so on ...).
 * https://oeis.org/A096309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + max( a(e) ) over all prime exponents e > 1 in n; a(n)
 *            = 1 for n <= 1
 * Range:     u8 n <= 9933, u16 n <= 9933, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;
    /* Factor n into prime powers.  exponents are *small*, so a recursive
       call on the exponent is inexpensive. */
    uint64_t m = (uint64_t)n;
    seq_t max_levels = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp > 1) {
                seq_t l = a((idx_t)exp);
                if (l > max_levels)
                    max_levels = l;
            }
        }
    }
    if (m > 1) {
        /* Remaining prime factor has exponent 1, i.e. no further
           levels to add */
    }
    return 1 + max_levels;
}
