/*
 * A138445  a(n) = ((n-th prime)^6-(n-th prime^2))/6.
 * https://oeis.org/A138445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n^6 - p_n^2)/6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_n is the n-th prime (p_1 = 2) */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, behaviour for n=0 not required */

    seq_t p  = (seq_t)get_prime(n);      /* n-th prime */
    seq_t p2 = p * p;                    /* p^2 */
    seq_t p4 = p2 * p2;                  /* p^4 */
    seq_t p6 = p4 * p2;                  /* p^6 */

    seq_t num = p6 - p2;                 /* p^6 - p^2 */
    seq_t res = num / 6;                 /* divide by 6 */

    return res;
}
