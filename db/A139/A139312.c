/*
 * A139312  Characteristic function of the good primes (version 1).
 * https://oeis.org/A139312
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if prime(n)^2 - prime(n-1)*prime(n+1) >= 0, else 0
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 2 */
    if (n < 2)
        return (seq_t)0;

    /* obtain the three consecutive primes */
    uint64_t p_prev = get_prime(n - 1);   /* prime(n-1)   */
    uint64_t p_cur  = get_prime(n);       /* prime(n)     */
    uint64_t p_next = get_prime(n + 1);   /* prime(n+1)   */

    /* compute difference with signed arithmetic to detect negativity */
    int64_t diff = (int64_t)p_cur * p_cur - (int64_t)p_prev * p_next;

    return diff >= 0 ? (seq_t)1 : (seq_t)0;
}
