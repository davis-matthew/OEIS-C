/*
 * A064216  Replace each p^e with prevprime(p)^e in the prime factorization of
 *          odd numbers; inverse of sequence A048673 considered as a
 *          permutation of the natural numbers.
 * https://oeis.org/A064216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A064989(2n – 1)
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064989(t) replaces each prime factor p^e of an odd number t
 * by (prevprime(p))^e.  */

/* The code is valid C11/C++17 and uses only integer arithmetic.  */

#include <stdint.h>
#include <stdlib.h>

/* ----- helper: test primality by trial division --------------------------------- */
static int is_prime_uint64(uint64_t x)
{
    if (x < 2) return 0;
    if (x == 2) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

/* ----- helper: previous prime of a prime p (p >= 3) ---------------------------- */
static uint64_t previous_prime_uint64(uint64_t p)
{
    /* p is guaranteed to be a prime and at least 3 */
    for (uint64_t q = p - 1; q >= 2; --q)
        if (is_prime_uint64(q))
            return q;
    return 2;          /* never reached for p >= 3 */
}

/* ----- main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Compute t = 2n – 1 (always odd).  n is idx_t (uint64_t). */
    uint64_t t = 2ULL * n - 1ULL;

    /* Special case: a(1) = 1 (t = 1). */
    if (t <= 1ULL)
        return (seq_t)1;

    uint64_t temp = t;
    seq_t result = 1;

    /* Factorization by trial division on odd divisors starting at 3. */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }

            /* Replace the prime factor p^exp by (prevprime(p))^exp. */
            uint64_t prev = previous_prime_uint64(p);
            seq_t term = 1;
            for (int i = 0; i < exp; ++i)
                term *= (seq_t)prev;

            result *= term;
        }
    }

    /* If a prime factor larger than sqrt(t) remains. */
    if (temp > 1ULL) {
        seq_t term = previous_prime_uint64(temp);
        result *= term;
    }

    return result;
}
