/*
 * A033229  Primes of form x^2+42*y^2.
 * https://oeis.org/A033229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 775, u32 n <= 776, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the *n*‑th prime that can be written as
   x^2 + 42 * y^2.  
   On the web page the sequence is:
      a(1) = 43, a(2) = 67, a(3) = 163, …  (OEIS A033229)
   A prime p has this form iff p ≡ 1, 25, 43, 67, 121 or 163 (mod 168).
   The routine keeps a cache so that calls may be made in any order. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but safe return */

    /* static cache of found terms */
    static seq_t *cache = NULL;      /* dynamically sized array of primes   */
    static size_t cache_cap = 0;     /* current allocation (number of items) */
    static idx_t  found = 0;         /* number of primes stored              */
    static seq_t last_candidate = 1; /* value of the last examined number    */

    while (found < n)
    {
        /* Advance to the next integer */
        last_candidate = last_candidate + 1;

        /* Skip numbers not in the allowed residue classes mod 168 */
        seq_t r = last_candidate % 168;
        if (!(r == 1 || r == 25 || r == 43 || r == 67 || r == 121 || r == 163))
            continue;

        /* Test primality; for arbitrary‑precision values we use the helper */
        if (!is_prime_big(last_candidate))
            continue;

        /* Ensure space in the cache */
        if ((size_t)(found + 1) > cache_cap)
            grow_seq(&cache, &cache_cap, (size_t)(found + 3)); /* allocate a bit extra */

        cache[found] = last_candidate;
        ++found;
    }

    return cache[n - 1];
}
