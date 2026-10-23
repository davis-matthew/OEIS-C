/*
 * A122257  Characteristic function of Pierpont primes (A005109).
 * https://oeis.org/A122257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a Pierpont prime (prime of the form 2^u * 3^v +
 *            1), else 0. Equivalent to: a(n) = 1 if n is prime and n-1 has no
 *            prime factors > 3, else 0
 * Range:     u8 n <= 9850, u16 n <= 9850, u32 n <= 9850, u64 n <= 9850,
 *            u128 n <= 9850, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;          /* offset starts at 1 */
    if (!is_prime(n)) return (seq_t)0;    /* not a prime */

    /* n-1 must be 3‑smooth: only prime divisors 2 or 3 */
    idx_t m = n - 1;
    while ((m % 2) == 0) m /= 2;
    while ((m % 3) == 0) m /= 3;

    return (m == 1) ? (seq_t)1 : (seq_t)0;
}
