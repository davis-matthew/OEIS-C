/*
 * A125137  a(n) = p^p + 1, where p = prime(n).
 * https://oeis.org/A125137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^p + 1, where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* undefined for n < 1, return 0 */

    seq_t p = (seq_t)get_prime(n);           /* prime(n) */
    seq_t res = big_pow(p, p);               /* compute p^p */
    res += (seq_t)1;                         /* add 1 */
    return res;
}
