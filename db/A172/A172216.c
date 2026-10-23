/*
 * A172216  Smallest k such that sum of digits of prime(n)^k is prime.
 * https://oeis.org/A172216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that the sum of digits of prime(n)^k is
 *            prime
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses decimal big‑integer multiplication so it works for any   */
/* seq_t width, including 64‑bit where prime^k may overflow native integers. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

/* Find the smallest exponent k so that the sum of digits of p^k is prime.  */
seq_t a(idx_t n)
{
    /* Retrieve the nth prime (1‑indexed). */
    uint32_t p = get_prime(n);
    /* Decimal representation of the current power, little‑endian. */
    unsigned char digits[256];
    size_t len = 1;
    digits[0] = 1;                    /* start with 1 (p^0) */

    for (idx_t k = 1; ; ++k) {
        /* Multiply the current value by p. */
        unsigned long long carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned long long prod = (unsigned long long)digits[i] * p + carry;
            digits[i] = prod % 10;
            carry = prod / 10;
        }
        while (carry) {
            digits[len++] = carry % 10;
            carry /= 10;
        }

        /* Compute the sum of digits. */
        unsigned sum = 0;
        for (size_t i = 0; i < len; ++i) sum += digits[i];

        /* Check if the sum is prime. */
        if (is_prime_small((seq_t)sum))
            return (seq_t)k;
    }
}
