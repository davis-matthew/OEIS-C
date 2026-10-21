/*
 * A073271  a(n) = floor( prime(n)*prime(n+2) / prime(n+1) ).
 * https://oeis.org/A073271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( prime(n)*prime(n+2) / prime(n+1) )
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* Undefined for n < 1, return 0 */
    seq_t p_n  = (seq_t)get_prime(n);      /* prime(n)  */
    seq_t p_np1 = (seq_t)get_prime(n+1);   /* prime(n+1)*/
    seq_t p_np2 = (seq_t)get_prime(n+2);   /* prime(n+2)*/
    return (p_n * p_np2) / p_np1;
}
