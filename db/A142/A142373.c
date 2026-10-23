/*
 * A142373  Primes congruent to 22 mod 47.
 * https://oeis.org/A142373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th prime ≡ 22 (mod 47) by incremental search */
/* Uses is_prime_small for primality tests and grow_seq for dynamic storage */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_small.h"

static seq_t *seq_cache = NULL;     /* dynamic array of found primes */
static size_t cache_cap = 0;        /* current capacity of seq_cache */
static idx_t next_candidate = 22;   /* next integer ≡ 22 (mod 47) to test */
static idx_t found = 0;             /* number of primes found so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    while (found < n) {
        if (is_prime_small(next_candidate)) {
            if (found >= cache_cap)
                grow_seq(&seq_cache, &cache_cap, (size_t)found + 1);
            seq_cache[found] = (seq_t)next_candidate;
            found++;
        }
        next_candidate += 47;
    }
    return seq_cache[n - 1];
}
