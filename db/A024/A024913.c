/*
 * A024913  Numbers k such that 10*k - 7 is prime.
 * https://oeis.org/A024913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by finding the nth prime congruent to 3 (mod 10)
   and returning (p+7)/10. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether n is prime.  n is small (< few million), so trial division
   is efficient enough. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence offset starts at 1 */

    uint64_t count = 0;
    uint64_t candidate = 3;        /* first number ≡ 3 (mod 10) */

    while (1)
    {
        if (is_prime_uint64(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)((candidate + 7) / 10);
        }
        candidate += 10;   /* stay in numbers ≡ 3 (mod 10) */
    }
}
