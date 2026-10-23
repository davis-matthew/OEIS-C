/*
 * A151918  a(n) = k! - prime(n) where k is the smallest number for which
 *          prime(n) <= k!.
 * https://oeis.org/A151918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A048765(prime(n)) - prime(n) (k! – p(n), smallest k with
 *            k! ≥ p(n))
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* prime(p_n) with zero‑based n: use (n+1)th prime */
    uint32_t p = get_prime(n + 1);

    /* find smallest k with k! ≥ p */
    unsigned long long fact = 1;
    unsigned long long k = 1;
    while (fact < (unsigned long long)p) {
        ++k;
        fact *= k;
    }

    /* compute difference; cast to seq_t – if it does not fit, it wraps as needed */
    unsigned long long diff = fact - (unsigned long long)p;
    return (seq_t)diff;
}
