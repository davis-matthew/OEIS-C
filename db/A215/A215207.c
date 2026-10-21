/*
 * A215207  Primes congruent to {2, 7} mod 13.
 * https://oeis.org/A215207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code computes OEIS A215207 (primes congruent to 2 or 7 mod 13)
   by iterating over integers and checking primality. */

#include "../utils/is_prime_small.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static size_t prime_cap = 0;
    static size_t prime_count = 0;
    static idx_t current = 2; /* next candidate to test */

    while (prime_count < n) {
        /* find next candidate that satisfies the congruence and is prime */
        while (true) {
            if ((current % 13ULL == 2ULL || current % 13ULL == 7ULL) &&
                is_prime_small(current)) {
                break;
            }
            ++current;
        }
        grow_seq(&primes, &prime_cap, prime_count + 1);
        primes[prime_count++] = (seq_t)current;
        ++current; /* advance to next integer */
    }
    return primes[n - 1];
}
