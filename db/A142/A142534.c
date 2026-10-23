/*
 * A142534  Primes congruent to 4 mod 53.
 * https://oeis.org/A142534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 4 (mod 53), offset 1
 * Range:     u8 n <= 1, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static idx_t candidate = 4;          // last checked candidate (starts at 4)
    static seq_t *cache      = NULL;     // array of found primes
    static size_t count     = 0;         // number of primes stored
    static size_t cap       = 0;         // current capacity of cache

    if (n == 0) return 0;               // undefined for n < 1, return 0

    if ((idx_t)n <= count) return cache[n - 1];

    /* Generate more primes until we have at least n of them */
    for (;;) {
        candidate += 53;                 /* next number ≡ 4 (mod 53) */
        if (is_prime(candidate)) {
            if (count >= cap)
                grow_seq(&cache, &cap, count + 1);
            cache[count++] = (seq_t)candidate;
            if (count >= n) break;
        }
    }
    return cache[n - 1];
}
