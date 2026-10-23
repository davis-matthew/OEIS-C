/*
 * A174237  Numbers n such that 4n-1 and 4n+3 are prime.
 * https://oeis.org/A174237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is searched as the n‑th integer such that 4n-1 and 4n+3 are
 *            prime
 * Range:     u8 n <= 10, u16 n <= 618, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_test = 1;          /* next candidate to test */

    while (cache_size < n)
    {
        seq_t cand = (seq_t)next_test;
        seq_t p1   = (seq_t)4 * cand - 1;
        seq_t p2   = (seq_t)4 * cand + 3;

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2))
        {
            if (cache_size == cache_cap)
            {
                cache_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cand;
        }

        if (next_test == 1)
            next_test = 2;        /* handle the special case n == 1 */
        else
            next_test += 3;       /* only numbers ≡ 2 (mod 3) can succeed thereafter */
    }

    return cache[n - 1];
}
