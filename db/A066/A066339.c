/*
 * A066339  Number of primes p of the form 4m+1 with p <= n.
 * https://oeis.org/A066339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p ≤ n such that p ≡ 1 (mod 4)
 * Range:     u8 n <= 3732, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* no primes ≤ 1 */

    seq_t count = 0;

    for (idx_t p = 2; p <= n; ++p) {
        /* fast elimination of even numbers */
        if ((p & 1) == 0) {
            /* only 2 is a prime, and 2 ≡ 2 (mod 4) → skip */
            continue;
        }

        /* trial division primality test for odd p */
        bool is_prime = true;
        for (idx_t d = 3; d * d <= p; d += 2) {
            if (p % d == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime) continue;

        /* p is prime; check the residue class */
        if ((p & 3) == 1) {
            count++;
        }
    }

    return count;
}
