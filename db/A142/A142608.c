/*
 * A142608  Primes congruent to 9 mod 55.
 * https://oeis.org/A142608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 9 (mod 55)
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset starts at 1 */

    static idx_t *primes = NULL;          /* vector of found primes   */
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t candidate = 9;           /* first number ≡ 9 (mod 55) */

    while (primes_len < n) {
        if (is_prime(candidate)) {
            if (primes_len == primes_cap) {
                size_t new_cap = (primes_cap == 0) ? 8 : (primes_cap << 1);
                idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp)
                    abort();              /* out of memory */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;
        }
        candidate += 55;                  /* next value ≡ 9 (mod 55) */
    }

    return (seq_t)primes[n - 1];
}
