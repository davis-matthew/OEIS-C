/*
 * A079054  a(n) = -1 if the closest prime to prime(n) is prime(n-1); = 1 if
 *          the closest prime to prime(n) is prime(n+1); = 0 if prime(n-1) and
 *          prime(n+1) are equally close to prime(n).
 * https://oeis.org/A079054
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = min(1, max(-1, prime(n-1)-prime(n)))
 * Range:     i8 n <= 9706, i16 n <= 9706, i32 n <= 9706, i64 n <= 9706,
 *            i128 n <= 9706, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A079054: the sign of the difference between
   prime(n-1) and prime(n), clamped to -1..1. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 2 */
    if (n < 2) return (seq_t)0;

    /* Retrieve the required primes */
    uint32_t p_prev = get_prime(n - 1);   /* prime(n-1)   */
    uint32_t p_cur  = get_prime(n);       /* prime(n)     */
    uint32_t p_next = get_prime(n + 1);   /* prime(n+1)   */

    /* Compute distances to neighboring primes */
    uint32_t diff_prev = p_cur - p_prev;  /* distance to previous prime */
    uint32_t diff_next = p_next - p_cur;  /* distance to next prime */

    /* Determine the closer prime */
    if (diff_prev < diff_next)
        return (seq_t)-1;      /* previous prime is closer */
    if (diff_prev > diff_next)
        return (seq_t)1;       /* next prime is closer */

    return (seq_t)0;           /* equidistant */
}
