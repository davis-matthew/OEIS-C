/*
 * A077648  Initial digits of prime numbers.
 * https://oeis.org/A077648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A000030(A000040(n)) – most significant decimal digit of
 *            the n‑th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; for n < 1 the sequence is undefined – return 0 */
    if (n < 1)
        return (seq_t)0;

    /* Retrieve the n‑th prime (returns a 32‑bit unsigned integer). */
    uint32_t p = get_prime(n);

    /* Extract the most significant decimal digit. */
    while (p >= 10u)
        p /= 10u;

    return (seq_t)p;
}
