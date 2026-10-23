/*
 * A142052  Primes congruent to 5 mod 33.
 * https://oeis.org/A142052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 5 (mod 33)
 * Range:     u8 n <= 3, u16 n <= 327, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static idx_t next_candidate = 5;   /* next number ≡ 5 (mod 33) */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t count = 0;

    while (count < n)
    {
        if (is_prime(next_candidate))
        {
            if (count >= cap)
            {
                size_t new_cap = cap ? cap << 1 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* memory exhaustion */
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = (seq_t)next_candidate;
        }
        next_candidate += 33;
    }

    return cache[n - 1];
}
