/*
 * A141856  Primes congruent to 9 mod 11.
 * https://oeis.org/A141856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 657, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth prime congruent to 9 (mod 22) by trial‑division search. */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.
   Returns true if n is prime, false otherwise. */
static bool is_prime_uint64(idx_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return (n == 2);
    for (idx_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 is defined; 0 is unused */

    idx_t count = 0;               /* how many primes found */
    idx_t candidate = 9;           /* start below the first valid prime */

    while (count < n) {
        candidate += 22;           /* next number ≡ 9 (mod 22) */
        if (is_prime_uint64(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
    }
    return 0;   /* unreachable */
}
