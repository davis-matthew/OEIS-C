/*
 * A079260  Characteristic function of primes of form 4n+1 (1 if n is prime of
 *          form 4n+1, 0 otherwise).
 * https://oeis.org/A079260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a prime of the form 4k+1; otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Returns 1 for a prime n ≡ 1 (mod 4), 0 otherwise. */
seq_t a(idx_t n)
{
    if (n < 2)                 /* numbers <2 are not prime */
        return (seq_t)0;

    /* 2 is the only even prime and it is not ≡1 (mod4) */
    if (n == 2)                /* 2 ≡ 2 (mod4) → 0 */
        return (seq_t)0;

    /* test odd divisors only */
    for (idx_t d = 3; d <= n / d; d += 2)
        if (n % d == 0)
            return (seq_t)0;   /* composite */

    /* n is prime */
    return (seq_t)((n % 4 == 1) ? 1 : 0);
}
