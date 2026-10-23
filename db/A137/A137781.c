/*
 * A137781  a(n) = (2^prime(n) + 2^prime(n+1)) / 4.
 * https://oeis.org/A137781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^prime(n) + 2^prime(n+1)) / 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow2.h"

/* Computes A137781 – a(n) = (2^prime(n) + 2^prime(n+1))/4.
 * Returns 0 for n == 0 (out of offset range). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for offset < 1 */

    seq_t p1 = pow2(get_prime(n));      /* 2^prime(n)   */
    seq_t p2 = pow2(get_prime(n + 1));  /* 2^prime(n+1) */

    return (p1 + p2) / ((seq_t)4);      /* divide by 4 */
}
