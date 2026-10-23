/*
 * A045314  Primes congruent to {2, 3, 5} (mod 7).
 * https://oeis.org/A045314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isprime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to {2, 3, 5} modulo 7, computed by incremental trial division. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isprime_u64.h"

/* Returns the n‑th prime ≡ 2,3,5 (mod 7), where a(1)=2. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* below offset – arbitrary */

    static seq_t *cache = NULL;      /* cached terms */
    static size_t  cap    = 0;        /* capacity of cache */
    static idx_t   cnt    = 0;        /* number of terms stored */
    static uint64_t next  = 2;        /* next integer to test */

    while (cnt < n)
    {
        uint64_t cand = next;
        /* Find the next prime with the required residue class. */
        do
        {
            if (isprime_u64(cand))
            {
                uint64_t m = cand % 7;
                if (m == 2 || m == 3 || m == 5) break;
            }
            ++cand;
        } while (1);

        /* Resize cache if needed. */
        if (cnt == cap)
        {
            size_t new_cap = cap == 0 ? 8 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();        /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        cache[cnt] = (seq_t)cand;
        ++cnt;
        next = cand + 1;              /* start next search after this prime */
    }

    return cache[n - 1];
}
