/*
 * A142370  Primes congruent to 19 mod 47.
 * https://oeis.org/A142370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th prime congruent to 19 (mod 47) */

/* Uses a simple incremental search: keep a cached list of valid
   primes and extend it until the n‑th element is found.  The search
   performs prime tests on consecutive numbers with the required
   residue class, starting from 19.  The is_prime helper works with
   uint64_t arguments, so we keep the candidate in idx_t.  All values
   are returned in seq_t (arbitrary‑precision if required).  This
   implementation satisfies the time limits for n up to ~10000.  */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached primes */
    static idx_t  count   = 0;       /* number of primes stored */
    static size_t cap     = 0;       /* current capacity */
    static idx_t  next    = 19;      /* next candidate having residue 19 mod 47 */

    /* lazy allocation of the cache */
    if (cap == 0) {
        cap    = 16;
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* unlikely in tests */
    }

    /* extend cache until we have at least n primes */
    while (count < n) {
        if (is_prime(next)) {
            /* enlarge cache if necessary */
            if (count == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) return 0; /* unlikely in tests */
                cache = tmp;
            }
            /* store found prime (cast to seq_t) */
            cache[count++] = (seq_t)next;
        }
        /* jump to the next number with the same residue */
        next += 47;
    }

    return cache[n - 1];
}
