/*
 * A075235  Primes whose base 5 reversal is also prime.
 * https://oeis.org/A075235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th prime p such that the base‑5 reversal of p is
 *            also prime
 * Range:     u8 n <= 31, u16 n <= 1327, u32 n <= 1327, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cap = 0;
    static uint64_t curr = 1;  /* start before the first prime */

    if (n == 0) return 0;  /* undefined for n < 1 */

    while (cached < n) {
        /* Get the next prime p */
        curr = next_prime(curr);
        uint64_t p = curr;

        /* Extract base‑5 digits of p (LSB first) */
        uint8_t digits[32];
        size_t dlen = 0;
        uint64_t tmp = p;
        if (tmp == 0) {
            digits[dlen++] = 0;
        } else {
            while (tmp > 0) {
                digits[dlen++] = tmp % 5;
                tmp /= 5;
            }
        }

        /* Reconstruct the reversed base‑5 number in seq_t */
        seq_t rev = 0;
        for (size_t i = 0; i < dlen; ++i) {
            rev = rev * 5 + digits[i];
        }

        /* Store p if both p and rev are prime */
        if (is_prime_big(rev)) {
            grow_seq(&cache, &cap, cached + 1);
            cache[cached++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
