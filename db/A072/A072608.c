/*
 * A072608  Parity of remainder Mod(prime(n),n) = A004648(n).
 * https://oeis.org/A072608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = Mod( Mod( prime(n), n), 2 )
 * Range:     u8 n <= 66, u16 n <= 7446, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 as per OEIS offset; return 0 for n=0 (undefined case) */
    if (n == 0)
        return 0;

    seq_t p = get_prime(n);        /* prime number p = prime(n) */
    seq_t rem = p % (seq_t)n;      /* remainder of p divided by n */
    return rem & 1;                /* parity of the remainder */
}
