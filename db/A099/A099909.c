/*
 * A099909  (prime(n)*(prime(n+1)-1) + (prime(n)-1)*prime(n+1)) / 2.
 * https://oeis.org/A099909
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)*(prime(n+1)-1)+(prime(n)-1)*prime(n+1))/2
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2; values <2 are unused */
    seq_t p_n   = get_prime(n);    /* p_n = prime(n)      */
    seq_t p_next = get_prime(n+1); /* p_{n+1} = prime(n+1) */
    seq_t prod = p_n * p_next;     /* product of the two primes   */
    seq_t sum  = p_n + p_next;     /* their sum                    */
    seq_t half = sum / 2;          /* integer division; sum is even */
    return prod - half;            /* a(n) = p_n*p_{n+1} - (p_n+p_{n+1})/2 */
}
