/*
 * A024701  a(n) = (-1 + prime(n+1)^2)/4.
 * https://oeis.org/A024701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n+1)^2 - 1) / 4
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper get_prime(k) from "../utils/get_prime.h" */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1-based; get n+1-th prime */
    uint32_t p = get_prime(n + 1);          /* p fits in 32 bits */
    seq_t  x = (seq_t)p * (seq_t)p;          /* p^2, promoted to seq_t */
    return (x - 1) / 4;                      /* (p^2 - 1) / 4 */
}
