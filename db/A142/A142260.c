/*
 * A142260  Primes congruent to 11 mod 43.
 * https://oeis.org/A142260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes ≡ 11 (mod 43) found by incremental search */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th prime congruent to 11 modulo 43 (a(1)=11). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, undefined for 0 */

    idx_t candidate = 11;                /* numbers of the form 11 + 43·k   */
    idx_t cnt = 0;                       /* how many such primes found     */

    while (cnt < n)
    {
        if (is_prime(candidate))          /* test primality of candidate    */
        {
            ++cnt;
            if (cnt == n)
                return (seq_t)candidate;   /* cast to seq_t for return value */
        }
        candidate += 43;
    }
    return 0; /* never reached, only to satisfy the compiler */
}
