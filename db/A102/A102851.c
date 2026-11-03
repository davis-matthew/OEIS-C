/*
 * A102851  Primes of the form 19n + 5.
 * https://oeis.org/A102851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 19k + 5 (OEIS A102851)
 * Range:     u8 n <= 4, u16 n <= 372, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* out‑of‑range request – return 0 */
        return (seq_t)0;

    unsigned long long candidate = 5ull;  /* the first number of the form 19k+5 */
    const unsigned long long step = 19ull;/* difference between consecutive candidates */

    idx_t count = 0;
    while (1) {
        /* test for primality */
        if (is_prime_u64(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += step;
    }
}
