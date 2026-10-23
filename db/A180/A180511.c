/*
 * A180511  Fermat quotients for base 4: (4^(p - 1) - 1)/p, where p =
 *          prime(n).
 * https://oeis.org/A180511
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^(p-1) - 1) / p , where p = prime(n) (1‑based)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for n < 2; return 0 */

    uint32_t p = get_prime(n);    /* nth prime (p ≥ 3 for n ≥ 2) */

    /* compute 4^(p-1) */
    seq_t num = big_pow((seq_t)4, (seq_t)(p - 1));

    /* subtract 1 and divide by p */
    num -= (seq_t)1;
    return num / (seq_t)p;
}
