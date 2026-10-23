/*
 * A142030  Primes congruent to 26 mod 31.
 * https://oeis.org/A142030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 26 (mod 31)
 * Range:     u8 n <= 1, u16 n <= 186, u32 n <= 187, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, but guard against n==0 */
    seq_t candidate = (seq_t)26;        /* first number ≡ 26 (mod 31) */
    idx_t count = 0;

    while (1) {
        if (is_prime_big(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate += (seq_t)31;        /* next number in the residue class */
    }
}
