/*
 * A086104  Primes appearing as concatenation of the last digit of
 *          prime(A086101(n)) and the first digit of prime(A086101(n)+1).
 * https://oeis.org/A086104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th prime formed by concatenating the last digit of
 *            the k‑th prime with the first digit of the (k+1)‑th prime for
 *            successive k that yield a prime
 * Range:     u8 n <= 25, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        seq_t pk = (seq_t)get_prime(k);
        seq_t pk_next = (seq_t)get_prime(k + 1);

        uint8_t last_digit = (uint8_t)(pk % 10);

        seq_t tmp = pk_next;
        while (tmp >= 10) tmp /= 10;
        uint8_t first_digit = (uint8_t)tmp;

        seq_t cand = (seq_t)last_digit * 10 + first_digit;
        if (is_prime_small(cand)) {
            ++count;
            if (count == n) return cand;
        }
        ++k;
    }
}
