/*
 * A173250  Positive odd nonprimes of the form prime-10.
 * https://oeis.org/A173250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p-10, where p is the next prime > 10 whose difference *
 *            p-10 is either 1 or a composite odd integer. The sequence is
 *            obtained * by brute‑forcing primes and filtering out those where
 *            p-10 is prime
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;        /* number of valid terms found */
    uint64_t p = 11;        /* first prime > 10 */

    while (1) {
        seq_t m = (seq_t)p - (seq_t)10;

        /* Include m if it is 1 or not prime. */
        if (m == 1 || !is_prime_big(m)) {
            ++count;
            if (count == n) {
                return m;
            }
        }

        /* Advance to the next prime after the current one. */
        p = next_prime(p + 1);
    }
}
