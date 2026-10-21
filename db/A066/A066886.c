/*
 * A066886  Sum of the elements in any transversal of a prime(n) X prime(n)
 *          array containing the numbers from 1 to prime(n)^2 in standard
 *          order.
 * https://oeis.org/A066886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)*(prime(n)^2+1)/2
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 308, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      (using the helper function get_prime(idx_t) from utils) */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1 */

    seq_t p = (seq_t)get_prime(n);   /* nth prime, promoted to seq_t */
    seq_t p2 = p * p;               /* p^2 */
    seq_t res = p * (p2 + (seq_t)1) / (seq_t)2;
    return res;
}
