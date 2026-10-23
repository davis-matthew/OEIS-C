/*
 * A142505  Primes congruent to 47 mod 51.
 * https://oeis.org/A142505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime that is congruent to 47 modulo 51
 * Range:     u8 n <= 3, u16 n <= 200, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Very small primality test – sufficient for the sizes involved (≈10⁶). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 1, return 0 be safe */
        return (seq_t)0;

    idx_t count = 1;            /* the first term 47 is already known */
    idx_t candidate = 47;       /* last prime found so far */

    while (count < n) {
        candidate += 51;        /* skip directly to the next number ≡47 (mod 51) */
        if (is_prime_uint64(candidate))
            ++count;
    }
    return (seq_t)candidate;
}
