/*
 * A142086  Primes congruent to 16 mod 35.
 * https://oeis.org/A142086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 16 (mod 35)
 * Range:     u8 n <= 1, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t count   = 0;
    static size_t capacity = 0;
    static idx_t candidate = 16;
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        candidate = 16;
    }

    while (count < (size_t)n) {
        candidate += 35;
        if ((candidate & 1U) == 0) continue;           /* skip even numbers */
        if (is_prime(candidate)) {
            grow_seq(&primes, &capacity, count + 1);
            primes[count++] = (seq_t)candidate;
        }
    }
    return primes[n - 1];
}
