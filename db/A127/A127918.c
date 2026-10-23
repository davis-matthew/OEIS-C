/*
 * A127918  Half of product of three numbers: n-th prime, previous and
 *          following number.
 * https://oeis.org/A127918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n * (p_n-1) * (p_n+1)) / 2 where p_n denotes the n‑th
 *            prime (p_1 = 2). The computation uses the provided utility
 *            function `uint32_t get_prime(idx_t k)`. All arithmetic is
 *            performed in type `seq_t`, which may be an unsigned integer or
 *            arbitrary‑precision integer as required by the build
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 309, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* Off‑by‑zero indices are undefined */

    seq_t p = (seq_t)get_prime(n);
    seq_t tmp = p * (p - 1u);        /* p*(p-1)  */
    tmp /= 2u;                      /* divide by 2 */
    return tmp * (p + 1u);          /* multiply by (p+1) */
}
