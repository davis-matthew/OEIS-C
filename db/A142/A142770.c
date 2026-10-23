/*
 * A142770  Primes congruent to 43 mod 59.
 * https://oeis.org/A142770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 43 modulo 59 (p % 59 == 43)
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t x) */
#include "../utils/grow_seq.h"  /* grow_seq(seq_t **, size_t *, size_t) */

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *primes = NULL;   /* array of primes */
    static size_t cap     = 0;     /* allocated capacity */
    static idx_t  len     = 0;     /* number of primes stored */
    static idx_t  cand    = 43;    /* current candidate to test */

    if (n == 0) return 0;          /* undefined for 0, ignore */

    /* Generate primes until we have at least n of them */
    while (len < (size_t)n) {
        /* test the current candidate */
        if (is_prime(cand)) {
            /* ensure capacity for the new prime */
            if (len == cap)
                grow_seq(&primes, &cap, len + 1);
            primes[len++] = (seq_t)cand;
        }
        cand += 59;                /* move to the next candidate of same residue */
    }

    return primes[n - 1];
}
