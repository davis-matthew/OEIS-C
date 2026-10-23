/*
 * A190867  Count of the 3-full divisors of n.
 * https://oeis.org/A190867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} max(1, e-1) (multiplicative with
 *            a(p^e)=max(1,e-1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t tmp = n;
    seq_t res = 1;
    if (tmp <= 1)
        return res; /* a(1) = 1 */

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t factor = (e > 1) ? (seq_t)(e - 1) : (seq_t)1;
            res *= factor;
        }
    }
    /* any remaining prime factor has exponent 1 ⇒ factor = 1, no effect on res */
    return res;
}
