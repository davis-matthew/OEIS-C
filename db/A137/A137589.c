/*
 * A137589  a(n) is the integer that results after deletion of all digits of
 *          n-th prime, except the initial digit and the final digit.
 * https://oeis.org/A137589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn, look
 * Formula:   a(n) is the integer obtained by concatenating the first and last
 *            decimal digits of the n‑th prime. For single‑digit primes the
 *            prime itself is returned. Calls are independent and may occur in
 *            any order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n must be >= 1; behavior for n < 1 is unspecified */
    uint32_t p = get_prime(n);          /* nth prime (1‑based) */

    /* single‑digit primes are handled specially */
    if (p < 10)
        return (seq_t)p;

    uint32_t last  = p % 10;            /* trailing digit */
    uint32_t first = p;
    while (first >= 10)                 /* isolate most significant digit */
        first /= 10;

    return (seq_t)(first * 10 + last);   /* concatenate first and last digit */
}
