/*
 * A171743  a(n) = A000010(A001043(n)).
 * https://oeis.org/A171743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = phi(prime(n) + prime(n+1))
 * Range:     u8 n <= 30, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   get_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1, but handle n < 1 defensively */
    if (n < 1)
        return 0;

    /* Retrieve the nth and (n+1)th primes */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* Sum the two primes (promote to idx_t to avoid overflow) */
    idx_t sum = (idx_t)p1 + (idx_t)p2;

    /* Euler's totient of the sum */
    return phi(sum);
}
