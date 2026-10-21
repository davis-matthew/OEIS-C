/*
 * A217920  Column 1 of A217916 (when formed into a number triangle).
 * https://oeis.org/A217920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n*(n-1)/2 + 1) - prime(n*(n-1)/2) (a(1)=2)
 * Range:     u8 n <= 286, u16 n <= 287, u32 n <= 287, u64 n <= 287,
 *            u128 n <= 287, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 2;

    /* Compute k = n*(n-1)/2 as an idx_t */
    idx_t k = (idx_t)(((seq_t)n * ((seq_t)n - 1)) / 2);

    /* Retrieve consecutive primes at positions k and k+1 */
    seq_t p_prev = get_prime(k);     /* prime(k)   */
    seq_t p_next = get_prime(k + 1); /* prime(k+1) */

    return p_next - p_prev;
}
