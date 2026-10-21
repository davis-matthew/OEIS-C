/*
 * A215209  Primes congruent to {2, 7} mod 19.
 * https://oeis.org/A215209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 549, u32 n <= 704, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215209: primes ≡ 2 or 7 (mod 19) – computed by incremental search. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;  /* stored primes */
    static size_t  cap    = 0;     /* current capacity */
    static size_t  sz     = 0;     /* number of primes stored */
    static idx_t   next   = 2;     /* next integer to test */

    /* Ensure we have at least n terms ready */
    while (sz < n)
    {
        seq_t cand = (seq_t)next;
        seq_t r    = cand % 19;
        if (r == 2 || r == 7)
        {
            if (is_prime_big(cand))
            {
                if (sz == cap)
                    grow_seq(&cache, &cap, sz + 1);
                cache[sz++] = cand;
            }
        }
        next++;
    }

    return cache[n - 1];
}
