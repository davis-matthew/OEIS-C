/*
 * A135171  3^p - 2^p, where p = prime(n).
 * https://oeis.org/A135171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^prime(n) - 2^prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   get_prime, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset starts at 1, defined 0 for safety */

    uint32_t p = get_prime(n);           /* get the n‑th prime (1‑based) */
    seq_t pow3p = pow3((idx_t)p);        /* 3^p */
    seq_t pow2p = pow2((idx_t)p);        /* 2^p */

    return pow3p - pow2p;                /* desired sequence term */
}
