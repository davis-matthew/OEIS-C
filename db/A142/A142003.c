/*
 * A142003  Primes congruent to 27 mod 29.
 * https://oeis.org/A142003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 210, u32 n <= 221, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142003 – primes congruent to 27 mod 29 (search). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset is 1 – n = 0 is undefined */

    static bool started = false;
    static seq_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t prime_cap = 0;
    static seq_t next_candidate = 0;

    if (!started) {
        started = true;
        next_candidate = (seq_t)27; /* will be incremented before first test */
    }

    /* generate primes until we have at least n of them */
    while (prime_count < n) {
        next_candidate += (seq_t)29;          /* to the next number ≡ 27 (mod 29) */
        if (is_prime_big(next_candidate)) {   /* primality test */
            grow_seq(&primes, &prime_cap, prime_count + 1);
            primes[prime_count] = next_candidate;
            prime_count++;
        }
    }

    return primes[n - 1];
}
