/*
 * A097933  Primes p that divide 3^((p-1)/2) - 1.
 * https://oeis.org/A097933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that p | 3^((p-1)/2) - 1;
 *            equivalently, p is a prime with p ≡ 1 or 11 (mod 12)
 * Range:     u8 n <= 25, u16 n <= 3270, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/grow_seq.h"   /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */

seq_t a(idx_t n)
{
    /* Cache of already found primes. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t next_candidate = 5;  /* last odd number tested; start from 5 */

    if (n == 0) return 0;   /* offset is 1; undefined for n==0 */

    /* Generate primes until we have at least n elements. */
    while (cache_size < n)
    {
        /* Only primes congruent to 1 or 11 modulo 12 can satisfy the condition. */
        if (next_candidate % 12 == 1 || next_candidate % 12 == 11)
        {
            if (is_prime(next_candidate))
            {
                /* Grow cache if needed. */
                if (cache_size == cache_cap)
                    grow_seq(&cache, &cache_cap, cache_size + 16);

                cache[cache_size] = (seq_t)next_candidate;
                cache_size++;
            }
        }
        next_candidate += 2;  /* skip even numbers */
    }

    return cache[n - 1];
}
