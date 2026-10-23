/*
 * A138182  Smallest summand in the Zeckendorf representation of the n-th
 *          prime.
 * https://oeis.org/A138182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A139764(A000040(n))
 * Range:     u8 n <= 50, u16 n <= 5473, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest Fibonacci summand in the Zeckendorf
 * representation of the n-th prime (1‑based). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Compute the n-th prime (n >= 1) using the provided helper. */
static inline uint32_t nth_prime(idx_t n)
{
    /* get_prime() is defined in utils/get_prime.h and returns the
     * n-th prime as a uint32_t.  It already handles the 1‑based index. */
    return get_prime(n);
}

/* Return the smallest Zeckendorf summand for the n‑th prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safeguard for out‑of‑range call */

    const uint32_t prime_uint = nth_prime(n);
    const seq_t prime = (seq_t)prime_uint;

    /* Generate Fibonacci numbers up to the prime.
       A small fixed buffer is sufficient – 64 elements covers all
       Fibonacci numbers less than 2^32. */
    seq_t fibs[64];
    size_t cnt = 0;
    fibs[cnt++] = (seq_t)1;
    fibs[cnt++] = (seq_t)2;

    while (true) {
        const seq_t next = fibs[cnt - 2] + fibs[cnt - 1];
        if (next > prime) break;
        fibs[cnt++] = next;
    }

    /* Greedy Zeckendorf decomposition to find the smallest summand. */
    seq_t rem = prime;
    seq_t min_summand = 0;          /* will hold the last (i.e. smallest) summand */

    for (size_t i = cnt; i > 0; --i) {
        const seq_t f = fibs[i - 1];
        if (f <= rem) {
            rem = rem - f;
            min_summand = f;        /* last chosen summand is the smallest */
            if (rem == 0) break;    /* complete decomposition */
        }
    }

    return min_summand;
}
