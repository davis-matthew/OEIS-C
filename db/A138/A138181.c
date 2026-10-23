/*
 * A138181  Largest Fibonacci number not exceeding the n-th prime.
 * https://oeis.org/A138181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest Fibonacci number not exceeding the n-th prime
 * Range:     u8 n <= 50, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* safety: offset starts at 1 */
    uint32_t p = get_prime(n);      /* n-th prime as a 32‑bit integer   */
    seq_t prime = (seq_t)p;         /* cast to the sequence type        */
    seq_t a_val = 0;                /* two successive Fibonacci terms   */
    seq_t b_val = 1;
    while (b_val <= prime) {        /* advance until we exceed the prime */
        seq_t tmp = b_val;
        b_val = a_val + b_val;
        a_val = tmp;
    }
    return a_val;                   /* largest Fibonacci <= nth prime   */
}
