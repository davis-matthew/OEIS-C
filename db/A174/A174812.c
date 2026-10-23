/*
 * A174812  Primes of the form n^2+42.
 * https://oeis.org/A174812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 34, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form n^2 + 42, returned in increasing n order. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

/* a(n) is the nth prime of the form i^2 + 42 (i >= 1,
   counting from n = 1). */
seq_t a(idx_t n)
{
    static idx_t last_checked = 0;      /* last i tested */
    static size_t cache_len = 0;        /* number of primes stored */
    static size_t cache_cap = 0;        /* allocated capacity */
    static seq_t *cache = NULL;         /* array of found primes */

    /* Grow cache until we have at least n terms */
    while (cache_len < n) {
        /* Next i */
        ++last_checked;

        /* Compute i^2 + 42 with overflow confined to seq_t */
        seq_t val = ((seq_t)last_checked) * ((seq_t)last_checked) + (seq_t)42;

        /* If the value is a prime, store it */
        if (is_prime_big(val)) {
            /* Ensure capacity for the new element */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = val;
        }
    }

    /* Return the nth term (n is 1‑based) */
    return cache[n - 1];
}
