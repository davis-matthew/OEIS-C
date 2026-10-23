/*
 * A132282  Near-cube primes: primes of the form p^3 + 2, where p is
 *          noncomposite.
 * https://oeis.org/A132282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p^3 + 2, where p is the n-th non‑composite value (0, 1,
 *            or a prime) for which p^3 + 2 itself is prime
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* is_prime(idx_t x) */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* Handle the first two trivial terms explicitly */
    if (n == 1) return (seq_t)2;   /* p = 0 -> 0^3 + 2 = 2 */
    if (n == 2) return (seq_t)3;   /* p = 1 -> 1^3 + 2 = 3 */

    /* Count how many valid near‑cube primes we have found so far. */
    idx_t count = 2;   /* already have the two terms above */
    idx_t p = 2;       /* start checking from the first prime */

    while (true) {
        if (is_prime(p)) {
            /* Compute p^3 + 2 */
            seq_t pt = (seq_t)p;
            seq_t val = pt * pt;   /* p^2 */
            val = val * pt + (seq_t)2;   /* p^3 + 2 */

            if (is_prime_big(val)) {
                ++count;
                if (count == n) return val;
            }
        }
        ++p;
    }
}
