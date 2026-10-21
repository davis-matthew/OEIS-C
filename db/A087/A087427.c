/*
 * A087427  Number of lattice points on or inside the rectangle formed by [1
 *          <= x <= (q-1)/2] and [1 <= y <= (p-1)/2], where p = n-th prime, q
 *          = (n-1)-st prime.
 * https://oeis.org/A087427
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n-1)-1)*(prime(n)-1)/4
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Obtain the (n-1)th and nth primes */
    uint32_t p_prev_u32 = get_prime(n-1);
    uint32_t p_curr_u32 = get_prime(n);

    seq_t p_prev = (seq_t)p_prev_u32;
    seq_t p_curr = (seq_t)p_curr_u32;

    /* Compute (p_prev-1)*(p_curr-1)/4 */
    seq_t val = (p_prev - 1) * (p_curr - 1);
    val /= (seq_t)4;

    return val;
}
