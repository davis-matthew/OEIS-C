/*
 * A038898  Primes p such that 23 is not a square mod p.
 * https://oeis.org/A038898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes p (p ≠ 23) such that 23 is a quadratic non‑residue
 *            modulo p (i.e. Legendre symbol (23|p) = –1)
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, jacobi_symbol, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/grow_seq_array.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;          /* array of cached primes   */
    static size_t  cache_len = 0;        /* number of stored elements */
    static size_t  cache_cap = 0;        /* allocated capacity       */

    static uint64_t last_checked = 1;    /* start position for next_prime */

    /* Grow the cache only when more terms are requested */
    while (cache_len < n)
    {
        /* Get next prime > last_checked */
        last_checked = next_prime(last_checked);   /* returns 2,3,5,... */

        /* Skip excluded primes */
        if (last_checked == 2 || last_checked == 23)
            continue;

        /* 23 is a quadratic non‑residue modulo last_checked? */
        if (jacobi_symbol(23, last_checked) == -1)
        {
            /* Ensure capacity for one more element */
            if (cache_len + 1 > cache_cap)
                grow_seq_array(&cache, &cache_cap, cache_len + 1);

            /* Store the prime (cast to seq_t) */
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
