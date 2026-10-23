/*
 * A166980  The smallest prime p larger than prime(n) such that prime(n) is a
 *          quadratic residue (mod p).
 * https://oeis.org/A166980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime, jacobi_symbol, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A166980  
    a(n) = the smallest prime p > prime(n) such that prime(n) is a quadratic residue mod p.
    (i.e. jacobi_symbol(prime(n), p) == 1) */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1 */
    if (n < 1)
        return (seq_t)0;

    /* nth prime */
    uint64_t q = (uint64_t)get_prime(n);

    /* search for the first prime > q with q a quadratic residue modulo that prime */
    uint64_t p = next_prime(q + 1);
    for (;;) {
        if (jacobi_symbol(q, p) == 1)
            return (seq_t)p;
        p = next_prime(p + 1);
    }
}
