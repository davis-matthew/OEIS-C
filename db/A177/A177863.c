/*
 * A177863  Product modulo p of the quadratic nonresidues of p, where p =
 *          prime(n).
 * https://oeis.org/A177863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) == (-1)^((prime(n)-1)/2) (mod prime(n))
 * Range:     u8 n <= 55, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* undefined index, defensively return 1 */

    uint32_t p32 = get_prime(n);   /* kth prime (k = n) */
    seq_t p = (seq_t)p32;

    if (p == 2)          /* empty product for p = 2 */
        return (seq_t)1;

    idx_t half = (idx_t)(p - 1) / 2;  /* (p-1)/2 */

    if ((half & 1) == 0)           /* even exponent: (-1)^even = 1 */
        return (seq_t)1;
    else                           /* odd exponent: (-1)^odd = -1 ≡ p-1 (mod p) */
        return p - 1;
}
